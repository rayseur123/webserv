<?php
echo "Content-Type: text/html; charset=utf-8\r\n";
echo "\r\n";
?>
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<title>test</title>
<link href="https://fonts.googleapis.com/css2?family=DM+Mono:wght@400&display=swap" rel="stylesheet">
<style>
* { margin: 0; padding: 0; box-sizing: border-box; }
body {
    background: #f7f6f3;
    font-family: 'DM Mono', monospace;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    min-height: 100vh;
    gap: 16px;
}
.card {
    background: #fff;
    border: 1px solid #e8e6e1;
    border-radius: 12px;
    padding: 32px 40px;
    text-align: center;
}
.dot {
    width: 10px; height: 10px;
    background: #4caf50;
    border-radius: 50%;
    display: inline-block;
    margin-right: 8px;
}
h1 { font-size: 1rem; font-weight: 400; color: #1a1916; }
p  { font-size: .72rem; color: #8a8780; margin-top: 8px; }
</style>
</head>
<body>
<div class="card">
    <h1><span class="dot"></span>ça marche</h1>
    <p>PHP <?= PHP_VERSION ?> · <?= date('d/m/Y H:i:s') ?></p>
</div>
</body>
</html>