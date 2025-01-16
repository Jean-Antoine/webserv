<?php
// Vérification que la requête est bien une POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Récupère le corps brut de la requête
    $rawData = file_get_contents("php://input");

    // Décoder le JSON
    $data = json_decode($rawData, true);

    // Vérification que le JSON est valide
    if (json_last_error() !== JSON_ERROR_NONE) {
        http_response_code(400); // Bad Request
        echo "Erreur : Format JSON invalide";
        exit;
    }

    // Récupérer les données du JSON
    $name = isset($data['name']) ? trim($data['name']) : '';
    $mail = isset($data['mail']) ? trim($data['mail']) : '';
    $message = isset($data['message']) ? trim($data['message']) : '';

    // Validation des données
    if (empty($name) || empty($mail) || empty($message)) {
        http_response_code(400); // Bad Request
        echo "Error: 'name', 'mail' and 'message' are mandatory";
        exit;
    }

    // Assainir les données pour éviter les failles
    $name = htmlspecialchars($name, ENT_QUOTES, 'UTF-8');
    $mail = htmlspecialchars($mail, ENT_QUOTES, 'UTF-8');
    $message = htmlspecialchars($message, ENT_QUOTES, 'UTF-8');

    // Définir le nom du fichier à créer
    $filename = "data_" . time() . ".txt";

    // Chemin du fichier (assurez-vous que le dossier a les bonnes permissions)
    $filepath = __DIR__ . '/uploads/' . $filename;

    // Contenu du fichier avec le message
    $content = "Name: $name\nEmail: $mail\nMessage: $message\n";

    // Écrire les données dans le fichier
    if (file_put_contents($filepath, $content) !== false) {
        http_response_code(201); // Created
        echo "Success creating data file: $filename";
    } else {
        http_response_code(500); // Internal Server Error
        echo "Error: impossible to create data file";
    }
} else {
    // Si la méthode n'est pas POST
    http_response_code(405); // Method Not Allowed
    echo "Error: method not allowed";
}
