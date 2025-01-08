<?php
// Default directory to list
$defaultDirectory = './';

// Get the directory from the query string, if provided
$directory = isset($_GET['dir']) ? $_GET['dir'] : die("Error: not directory set.");

// Flag to determine if the parent directory link should be included
$includeParent = isset($_GET['includeParent']) && $_GET['includeParent'] === 'true';

// Open the directory
if (!is_dir($directory)) {
	die("Error: Directory not found or not accessible.");
}

$files = scandir($directory);
if ($files === false) {
	die("Error: Unable to read directory.");
}

// Generate HTML for directory listing
?>
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Directory Listing</title>
	<style>
		body {
			font-family: Arial, sans-serif;
			margin: 20px;
		}
		table {
			width: 100%;
			border-collapse: collapse;
		}
		th, td {
			border: 1px solid #ddd;
			padding: 8px;
			text-align: left;
		}
		th {
			background-color: #f4f4f4;
		}
		a {
			text-decoration: none;
			color: #007BFF;
		}
		a:hover {
			text-decoration: underline;
		}
	</style>
</head>
<body>
	<h1>Directory Listing for '<?php echo htmlspecialchars($directory); ?>'</h1>
	<table>
		<thead>
			<tr>
				<th>Name</th>
				<th>Type</th>
				<th>Size</th>
				<th>Last Modified</th>
			</tr>
		</thead>
		<tbody>
			<?php foreach ($files as $file): ?>
				<?php if ($file === '.' || ($file === '..' && !$includeParent)) continue; ?>
				<?php $filePath = $directory . DIRECTORY_SEPARATOR . $file; ?>
				<tr>
					<td>
						<a href="<?php echo htmlspecialchars($file); ?>">
							<?php echo htmlspecialchars($file); ?>
						</a>
					</td>
					<td>
						<?php echo is_dir($filePath) ? 'Directory' : 'File'; ?>
					</td>
					<td>
						<?php echo is_file($filePath) ? filesize($filePath) . ' bytes' : '-'; ?>
					</td>
					<td>
						<?php echo date("Y-m-d H:i:s", filemtime($filePath)); ?>
					</td>
				</tr>
			<?php endforeach; ?>
		</tbody>
	</table>
</body>
</html>
