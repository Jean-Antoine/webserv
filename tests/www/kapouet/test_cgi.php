#!/usr/bin/env php
<?php
// Send the HTTP headers, ensuring CRLF separation
header("Content-Type: text/html\r\n");

// Start outputting HTML content
echo <<<HTML
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CGI PHP Script</title>
</head>
<body>
    <h1>Hello from a CGI Script!</h1>
    <p>This is a simple CGI script written in PHP.</p>
    <form method="post" action="test-post.php">
        <label for="name">Enter your name:</label>
        <input type="text" id="name" name="name">
        <button type="submit">Submit</button>
    </form>
HTML;

// Handle POST data
if ($_SERVER['REQUEST_METHOD'] === 'POST' && !empty($_POST['name'])) {
    $name = htmlspecialchars($_POST['name']); // Sanitize input
    echo "<p>Hello, <strong>$name</strong>!</p>";
}

echo <<<HTML
</body>
</html>
HTML;
?>
