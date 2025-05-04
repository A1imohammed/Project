import csv

def analyze_temperature_csv(file_path):
    temperatures = []

    with open(file_path, 'r') as file:
        reader = csv.reader(file)
        next(reader)

        for row in reader:
            try:
                temp = float(row[1].strip('"'))
                temperatures.append(temp)
            except (ValueError, IndexError):
                continue

    if not temperatures:
        print("No valid temperature data found.")
        return

    avg = sum(temperatures) / len(temperatures)
    print(f"Readings found: {len(temperatures)}")
    print(f"Average temperature: {avg:.2f} °C")
    print(f"Minimum temperature: {min(temperatures):.2f} °C")
    print(f"Maximum temperature: {max(temperatures):.2f} °C")


analyze_temperature_csv("temperature_feed.csv")

