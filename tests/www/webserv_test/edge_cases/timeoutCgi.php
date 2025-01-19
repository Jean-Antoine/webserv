<?php
set_time_limit(0); // Permet au script de ne pas expirer à cause d'une limite de temps

header("Content-Type: text/plain"); // Définit le type de contenu en texte brut

echo "Début de la boucle PHP infinie...\n";
flush(); // Force l'envoi des données au client immédiatement

$count = 0;
while (true) {
    $count++;
    echo "Iteration $count\n";
    flush(); // Force l'envoi des données à chaque itération
    sleep(1); // Pause de 1 seconde entre chaque itération
}
?>