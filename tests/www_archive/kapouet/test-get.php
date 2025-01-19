<?php
// Chemin vers le dossier contenant les fichiers
$dataDir = __DIR__ . '/data';

// Vérifie que le dossier existe
if (!is_dir($dataDir)) {
    die("Error: The data directory does not exist");
}

// Récupère tous les fichiers .txt dans le dossier
$dataFiles = glob($dataDir . '/data_*.txt');

// Fonction pour lire et formater le contenu d'un fichier
function readFileContent($filePath) {
    $content = file_get_contents($filePath);
    $lines = explode("\n", trim($content));
    
    $data = [];
    foreach ($lines as $line) {
        list($key, $value) = explode(":", $line, 2);
        $data[trim($key)] = trim($value);
    }
    return $data;
}
?>
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

    <?php if (empty($dataFiles)): ?>
        <p>No messages found</p>
    <?php else: ?>
        <div class="file-list">
            <?php foreach ($dataFiles as $file): ?>
                <?php $data = readFileContent($file); ?>
                <div class="file-entry">
                    <h2>Message stored in <?= basename($file) ?></h2>
                    <p><strong>Name:</strong> <?= htmlspecialchars_decode($data['Name'] ?? 'Unknown') ?></p>
                    <p><strong>Email:</strong> <?= htmlspecialchars_decode($data['Email'] ?? 'Unknown') ?></p>
                    <p><strong>Message:</strong> <?= nl2br(htmlspecialchars_decode($data['Message'] ?? 'No message')) ?></p>
                </div>
            <?php endforeach; ?>
        </div>
    <?php endif; ?>
</body>
</html>
