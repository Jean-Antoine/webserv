<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Récupérer les champs texte
    $name = isset($_POST['name']) ? trim($_POST['name']) : '';
    $mail = isset($_POST['mail']) ? trim($_POST['mail']) : '';
    $message = isset($_POST['message']) ? trim($_POST['message']) : '';

    // Valider les données
    if (empty($name) || empty($mail) || empty($message)) {
        http_response_code(400); // Bad Request
        echo "Erreur : Tous les champs sont obligatoires";
        exit;
    }

    // Assainir les données
    $name = htmlspecialchars($name, ENT_QUOTES, 'UTF-8');
    $mail = htmlspecialchars($mail, ENT_QUOTES, 'UTF-8');
    $message = htmlspecialchars($message, ENT_QUOTES, 'UTF-8');

    // Traitement du fichier uploadé
    $uploadDir = __DIR__ . '/uploads/';
    if (!is_dir($uploadDir)) {
        mkdir($uploadDir, 0755, true); // Créer le dossier si nécessaire
    }

    if (isset($_FILES['file']) && $_FILES['file']['error'] === UPLOAD_ERR_OK) {
        $uploadedFile = $_FILES['file'];
        $filePath = $uploadDir . basename($uploadedFile['name']);
        
        if (move_uploaded_file($uploadedFile['tmp_name'], $filePath)) {
            $fileMessage = "Fichier uploadé avec succès : " . $filePath;
        } else {
            $fileMessage = "Erreur lors de l'upload du fichier";
        }
    } else {
        $fileMessage = "Aucun fichier uploadé";
    }

    // Sauvegarder les données dans un fichier
    $dataFile = $uploadDir . 'data_' . time() . '.txt';
    $content = "Name: $name\nEmail: $mail\nMessage: $message\n$fileMessage\n";
    if (file_put_contents($dataFile, $content) !== false) {
        http_response_code(201); // Created
        echo "Données sauvegardées: $dataFile<br>$fileMessage";
    } else {
        http_response_code(500); // Internal Server Error
        echo "Erreur : Impossible de sauvegarder les données";
    }
} else {
    http_response_code(405); // Method Not Allowed
    echo "Erreur : Méthode non autorisée";
}