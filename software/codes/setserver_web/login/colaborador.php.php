<?php
	session_start();
	echo "Usuario: ". $_SESSION['usuarioNome'];	
?>
<br>
<a href="sair.php">Sair</a>