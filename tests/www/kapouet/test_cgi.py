#!/usr/bin/env python3

import cgi
import cgitb

# Enable debugging for the script (helpful during development)
cgitb.enable()

# Print the HTTP headers, ensuring CRLF separation
print("Content-Type: text/html\r\n\r\n", end="")  # Explicit CRLF separation

# HTML content
print("""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CGI Python Script</title>
</head>
<body>
    <h1>Hello from a CGI Script!</h1>
    <p>This is a simple CGI script written in Python.</p>
    <form method="post" action="test-post.php">
        <label for="name">Enter your name:</label>
        <input type="text" id="name" name="name">
        <button type="submit">Submit</button>
    </form>
""")

# Handle POST data
form = cgi.FieldStorage()
if "name" in form:
    name = form.getvalue("name")
    print(f"<p>Hello, <strong>{name}</strong>!</p>")

print("""
</body>
</html>
""")
