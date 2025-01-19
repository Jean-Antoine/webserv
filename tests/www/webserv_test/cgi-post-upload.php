<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Récupérer les champs texte
    $name = isset($_POST['name']) ? trim($_POST['name']) : '';
    $mail = isset($_POST['mail']) ? trim($_POST['mail']) : '';
    $message = isset($_POST['message']) ? trim($_POST['message']) : '';

    // Valider les données
    if (empty($name) || empty($mail) || empty($message)) {
        http_response_code(400); // Bad Request
        echo "Error: 'name', 'mail' and 'message' are mandatory";
        exit;
    }

    // Assainir les données
    $name = htmlspecialchars($name, ENT_QUOTES, 'UTF-8');
    $mail = htmlspecialchars($mail, ENT_QUOTES, 'UTF-8');
    $message = htmlspecialchars($message, ENT_QUOTES, 'UTF-8');

function uploadErrorToText($errorCode) {
    switch ($errorCode) {
        case UPLOAD_ERR_OK:
            return "No error, the file was uploaded successfully.";
        case UPLOAD_ERR_INI_SIZE:
            return "The uploaded file exceeds the upload_max_filesize directive in php.ini.";
        case UPLOAD_ERR_FORM_SIZE:
            return "The uploaded file exceeds the MAX_FILE_SIZE directive specified in the HTML form.";
        case UPLOAD_ERR_PARTIAL:
            return "The file was only partially uploaded.";
        case UPLOAD_ERR_NO_FILE:
            return "No file was uploaded.";
        case UPLOAD_ERR_NO_TMP_DIR:
            return "Missing a temporary folder.";
        case UPLOAD_ERR_CANT_WRITE:
            return "Failed to write file to disk.";
        case UPLOAD_ERR_EXTENSION:
            return "A PHP extension stopped the file upload.";
        default:
            return "Unknown error (code $errorCode).";
    }
}

    

    // Traitement du fichier uploadé
    $uploadDir = __DIR__ . '/uploads/';
    if (!is_dir($uploadDir)) {
        mkdir($uploadDir, 0755, true); // Créer le dossier si nécessaire
    }

    if (isset($_FILES['file']) && $_FILES['file']['error'] === UPLOAD_ERR_OK) {
        $uploadedFile = $_FILES['file'];
        $filePath = $uploadDir . basename($uploadedFile['name']);
        
        if (move_uploaded_file($uploadedFile['tmp_name'], $filePath)) {
            $fileMessage = "Success uploading file: " . basename($uploadedFile['name']);
        } else {
            $fileMessage = "Error: impossible to upload file: {$_FILES['file']['error']}";
        }
    } elseif ($_FILES['file']['error'] === UPLOAD_ERR_NO_FILE) {
        $fileMessage = "No file uploaded";
    }
    else {
        $fileMessage = "Error uploading file: ".uploadErrorToText($_FILES['file']['error']);
    }

    $dataDir = __DIR__ . '/data/';
    // Définir le nom du fichier à créer
    $filename = "data_" . time() . ".txt";

    // Chemin du fichier (assurez-vous que le dossier a les bonnes permissions)
    $filepath = $dataDir . $filename;

    // Contenu du fichier avec le message
    $content = "Name: $name\nEmail: $mail\nMessage: $message\n";

    // Écrire les données dans le fichier
    if (file_put_contents($filepath, $content) !== false) {
        http_response_code(201); // Created
        echo "Success creating data file: $filename\n",
            "$fileMessage";
    } else {
        http_response_code(500); // Internal Server Error
        echo "Error: impossible to create data file";
    }
} else {
    // Si la méthode n'est pas POST
    http_response_code(405); // Method Not Allowed
    echo "Error: method not allowed";
}
