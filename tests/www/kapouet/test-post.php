<?php
// Vérification que la requête est bien une POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Récupérer les données du formulaire
    $name = isset($_POST['name']) ? trim($_POST['name']) : '';
    $mail = isset($_POST['mail']) ? trim($_POST['mail']) : '';
    $message = isset($_POST['message']) ? trim($_POST['message']) : '';

    // Validation des données
    if (empty($name) || empty($mail) || empty($message)) {
        http_response_code(400); // Bad Request
        echo "Erreur : 'name', 'mail', et 'message' sont obligatoires.";
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
        echo "Fichier créé avec succès : $filename";
    } else {
        http_response_code(500); // Internal Server Error
        echo "Erreur : impossible de créer le fichier.";
    }
} else {
    // Si la méthode n'est pas POST
    http_response_code(405); // Method Not Allowed
    echo "Erreur : cette méthode n'est pas autorisée.";
}
