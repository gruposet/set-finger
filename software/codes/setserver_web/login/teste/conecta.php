<?php
	session_start();	
	
	if((isset($_POST['usuario'])) && (isset($_POST['senha']))){
		
		}else{	
			//Váriavel global recebendo a mensagem de erro
			$_SESSION['loginErro'] = "Usuário ou senha Inválido";
			header("Location: index.php");
		}
	
?>