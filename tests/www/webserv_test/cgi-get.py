#!/usr/bin/env python3

import cgi
import cgitb
import os

# Enable debugging (helpful during development)
cgitb.enable()

# Get the directory of the current script
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_DIR = os.path.join(CURRENT_DIR, "data")  # Path to the data directory

# Function to read and parse file content
def read_file_content(file_path):
    try:
        with open(file_path, "r", encoding="utf-8") as f:
            lines = f.readlines()
        data = {}
        for line in lines:
            if ":" in line:
                key, value = line.split(":", 1)
                data[key.strip()] = value.strip()
        return data
    except Exception as e:
        return {"Error": f"Failed to read file: {str(e)}"}

# Generate HTML header
print("Content-Type: text/html\r\n\r\n", end="")

# Begin HTML content
print("""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Message listing</title>
	<link rel="stylesheet" href="/styles/listing_data.css">
</head>
<body>
    <h1>List of messages</h1>
""")

# Check if the data directory exists
if not os.path.isdir(DATA_DIR):
    print("<p>Error: The data directory does not exist</p>")
else:
    # Get all files in the directory that match "data_*.txt"
    files = [os.path.join(DATA_DIR, f) for f in os.listdir(DATA_DIR) if f.startswith("data_") and f.endswith(".txt")]

    if not files:
        print("<p>No messages found</p>")
    else:
        print(f"<div class=\"file-list\">")
        # Display each file's content
        for file_path in files:
            data = read_file_content(file_path)
            print(f"<div class=\"file-entry\">")
            print(f"<h2>Message stored in {os.path.basename(file_path)}</h2>")
            print(f"<p><strong>Name:</strong> {data.get('Name', 'Unknown')}</p>")
            print(f"<p><strong>Email:</strong> {data.get('Email', 'Unknown')}</p>")
            print(f"<p><strong>Message:</strong> {data.get('Message', 'No message')}</p>")
            if "Error" in data:
                print(f"<p><strong>Error:</strong> {data['Error']}</p>")
            print(f"</div>")
        print(f"</div>")

# End HTML content
print("""
</body>
</html>
""")
