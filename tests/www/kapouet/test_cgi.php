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
</body>

HTML;

echo <<<HTML
</body>
</html>
HTML;
