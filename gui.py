import tkinter as tk
from tkinter import ttk, filedialog, colorchooser
from PIL import Image, ImageTk
from geopy.distance import geodesic  # You may need to install geopy using: pip install geopy

class WorldMapApp:
    def __init__(self, root):
        self.root = root
        self.root.title("World Map")
        self.root.geometry("800x600")

        self.canvas = tk.Canvas(root, width=800, height=400)
        self.canvas.pack()

        self.cities = {}
        self.line_color = "blue"

        load_button = ttk.Button(root, text="Load Map Image", command=self.load_map_image)
        load_button.pack()

        draw_lines_button = ttk.Button(root, text="Draw Lines", command=self.draw_lines)
        draw_lines_button.pack()

        clear_lines_button = ttk.Button(root, text="Clear Lines", command=self.clear_lines)
        clear_lines_button.pack()

        color_button = ttk.Button(root, text="Select Line Color", command=self.choose_line_color)
        color_button.pack()

        self.city_info_label = ttk.Label(root, text="")
        self.city_info_label.pack()

        #self.city_entry_label = ttk.Label(root, text="Enter 4 city names (comma-separated):")
        #self.city_entry_label.pack()

        #self.city_entry = ttk.Entry(root)
        #self.city_entry.pack()

    def load_map_image(self):
        file_path = filedialog.askopenfilename(filetypes=[("Image files", "*.png *.jpg *.jpeg *.gif *.bmp")])

        if file_path:
            self.map_image = Image.open(file_path)
            self.map_image = self.map_image.resize((800, 400), Image.ANTIALIAS)
            self.map_photo = ImageTk.PhotoImage(self.map_image)
            self.canvas.create_image(0, 0, anchor=tk.NW, image=self.map_photo)

            self.canvas.delete("city_marker")
            self.load_cities()

    def load_cities(self):
        self.cities = {
            'taiwan': (25.0320, 121.5654),
            'marseille': (43.2965, 5.3698),
            'miami': (25.7617, -80.1918),
            'seattle': (47.6062, -122.3321),
            'osaka': (34.6937, 135.5023),
            'montreal': (45.5017, -73.5673),
            'jakarta': (-6.2088, 106.8456),
            'geneva': (46.2044, 6.1432),
            'Kuala Lumpur': (3.1390, 101.6869),
            'madrid': (40.4168, -3.7038),
            'frankfurt': (50.1109, 8.6821),
            'london': (51.5074, -0.1278),
            'mexico': (19.4326, -99.1332),
            'telAviv': (32.0853, 34.7818),
            'new york': (40.7128, -74.0060),
            'cairo': (30.0444, 31.2357),
            'seoul': (37.5665, 126.9780),
            'chennai': (13.0827, 80.2707),
            'los angeles': (34.0522, -118.2437),
            'milan': (45.4642, 9.1900),
            'lisbon': (38.7223, -9.1393),
            'Hong Kong': (22.3193, 114.1694),
            'moscow': (55.7558, 37.6176),
            'mumbai': (19.0760, 72.8777),
            'delhi': (28.6139, 77.2090),
            'singapore': (1.3521, 103.8198),
            'stockholm': (59.3293, 18.0686),
            'dubai': (25.276987, 55.296249),
            'tokyo': (35.682839, 139.759455),
            'paris': (48.8566, 2.3522),
            'perth': (-31.950527, 115.860457),
            'sao paulo': (-23.550520, -46.633308),
            'nairobi': (-1.2921, 36.8219),
            'brussels': (50.8503, 4.3517),
            'caracas': (10.4806, -66.9036),
            'johannesburg': (-26.2041, 28.0473),
            'dallas': (32.7767, -96.7970),
            'melbourne': (-37.8136, 144.9631),
            'warsaw': (52.2297, 21.0122),
            'des moines': (41.5868, -93.6250),
            'dublin': (53.349805, -6.26031),
            'santiago': (-33.4489, -70.6693),
            'berlin': (52.5200, 13.4050),
        }

        map_width, map_height = self.map_image.size

        for city, (lat, lon) in self.cities.items():
            # Calculate the position of the city dot based on the size of the resized map image
            x = (lon + 180) * (map_width / 360)
            y = map_height - (lat + 90) * (map_height / 180)

            self.canvas.create_oval(x - 5, y - 5, x + 5, y + 5, fill="red", tags=("city_marker", city))

    def draw_lines(self):
        f = open("result.txt", "r")
        f = f.readlines()
        city_names = f[0].split(",")
        city_names = city_names[:-1]
        #city_names = self.city_entry.get().split(',')
        city_names = [city.strip() for city in city_names]
        print(city_names)
        # if len(city_names) == 0 and all(city in self.cities for city in city_names):
        if True:
            self.canvas.delete("line")
            total_distance = float(f[1])

            for i in range(len(city_names) - 1):
                city1 = self.cities[city_names[i]]
                city2 = self.cities[city_names[i + 1]]
                x1, y1 = self.convert_coordinates_to_canvas(city1)
                x2, y2 = self.convert_coordinates_to_canvas(city2)
                self.canvas.create_line(x1, y1, x2, y2, fill=self.line_color, tags="line", width=2)

                # distance = geodesic(city1, city2).kilometers
                # total_distance += distance

                self.city_info_label.config(text=f"Total Time: {total_distance:.2f} ms")
        else:
            self.city_info_label.config(text="Invalid input. Please enter 4 valid city names.")

    def convert_coordinates_to_canvas(self, coordinates):
        lat, lon = coordinates
        map_width, map_height = self.map_image.size
        x = (lon + 180) * (map_width / 360)
        y = map_height - (lat + 90) * (map_height / 180)
        return x, y

    def clear_lines(self):
        self.canvas.delete("line")
        self.city_info_label.config(text="")
        self.city_entry.delete(0, tk.END)

    def choose_line_color(self):
        color = colorchooser.askcolor(title="Choose Line Color")[1]
        if color:
            self.line_color = color

if __name__ == "__main__":
    root = tk.Tk()
    app = WorldMapApp(root)
    root.mainloop()
