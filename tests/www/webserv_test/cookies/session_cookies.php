<?php
session_start(); // Démarrer ou reprendre une session

// Vérifier si le panier existe déjà, sinon l'initialiser
if (!isset($_SESSION['cart'])) {
    $_SESSION['cart'] = [];
}

// Ajouter un produit au panier
if (isset($_POST['add_to_cart'])) {
    $product = $_POST['product'];
    if (!in_array($product, $_SESSION['cart'])) { // Éviter les doublons
        $_SESSION['cart'][] = $product;
    }
}

// Supprimer un produit du panier
if (isset($_POST['remove_from_cart'])) {
    $product = $_POST['product'];
    if (($key = array_search($product, $_SESSION['cart'])) !== false) {
        unset($_SESSION['cart'][$key]);
    }
    $_SESSION['cart'] = array_values($_SESSION['cart']); // Réindexer les indices
}

// Réinitialiser le panier
if (isset($_POST['clear_cart'])) {
    $_SESSION['cart'] = [];
}

// Récupérer les produits dans le panier
$cart = $_SESSION['cart'];
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Shopping Cart with Session Cookies</title>
    <link rel="stylesheet" href="/styles/session_cookies.css">
</head>
<body>
    <h1>Shopping Cart with Session Cookies</h1>

    <div class="products">
        <h2>Products</h2>
        <form method="POST">
            <input type="hidden" name="product" value="Product A">
            <button type="submit" name="add_to_cart">Add Product A</button>
        </form>
        <form method="POST">
            <input type="hidden" name="product" value="Product B">
            <button type="submit" name="add_to_cart">Add Product B</button>
        </form>
        <form method="POST">
            <input type="hidden" name="product" value="Product C">
            <button type="submit" name="add_to_cart">Add Product C</button>
        </form>
    </div>

    <div class="cart">
        <h2>Your Cart</h2>
        <?php if (empty($cart)): ?>
            <p class="empty-cart">Your cart is empty.</p>
        <?php else: ?>
            <ul>
                <?php foreach ($cart as $item): ?>
                    <li class="cart-item">
                        <span><?= htmlspecialchars($item) ?></span>
                        <form method="POST" style="display:inline;">
                            <input type="hidden" name="product" value="<?= htmlspecialchars($item) ?>">
                            <button type="submit" class="remove-button" name="remove_from_cart">Remove</button>
                        </form>
                    </li>
                <?php endforeach; ?>
            </ul>
            <form method="POST">
                <button type="submit" class="remove-button" name="clear_cart">Clear Cart</button>
            </form>
        <?php endif; ?>
    </div>

    <div class="container">
        <a href="/webserv_test/cookies/session_cookies.php">Refresh</a>
    </div>

</body>
</html>
