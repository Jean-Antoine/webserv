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
        if (isset($_SESSION['session_test'])) {
            $session_value = $_SESSION['session_test'];
            echo "<h1 class='detected'>Session detected!</h1>";
            echo "<p>The current value of the cookie \"session_test\" is:</p>";
            echo "<p class='timestamp'>$session_value</p>";
            
            // Refresh the session with a new timestamp
            $ts = time();
            $_SESSION['session_test'] = $ts;
            echo "<p>The session value has been updated with a new timestamp:</p>";
            echo "<p class='timestamp'>$ts</p>";
        } else {
            // Set a new session with the current timestamp
            $ts = time();
			$_SESSION['session_test'] = $ts;
            echo "<h1 class='not-detected'>No session detected</h1>";
            echo "<p>We couldn't find the session \"session_test\"</p>";
            echo "<p>The session is initialized with timestamp:</p>";
            echo "<p class='timestamp'>$ts</p>";
        }
        ?>
	</div>
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
	</div>
</body>
</html>

