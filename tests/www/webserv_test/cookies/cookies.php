<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Cookie Test</title>
	<link rel="stylesheet" href="/styles/cookies.css">
</head>
<body>
	<div class="container">
        <?php
        if (isset($_COOKIE['test_cookie'])) {
            $cookie_value = $_COOKIE['test_cookie'];
            echo "<h1 class='detected'>Cookie detected!</h1>";
            echo "<p>The current value of the cookie \"test_cookie\" is:</p>";
            echo "<p class='timestamp'>$cookie_value</p>";
            
            // Refresh the cookie with a new timestamp
            $ts = time();
            setcookie('test_cookie', $ts, $ts + 30); // Cookie valable 30 secondes
            echo "<p>This cookie has been updated with a new timestamp:</p>";
            echo "<p class='timestamp'>$ts</p>";
        } else {
            // Set a new cookie with the current timestamp
            $ts = time();
            setcookie('test_cookie', $ts, $ts + 30); // Cookie valable 30 secondes
            echo "<h1 class='not-detected'>No cookie detected</h1>";
            echo "<p>We couldn't find the cookie \"test_cookie\"</p>";
            echo "<p>A new cookie has been created with the value:</p>";
            echo "<p class='timestamp'>$ts</p>";
        }
        ?>
    <a href="/webserv_test/cookies/cookies.php">Refresh</a>
	</div>
</body>
</html>

