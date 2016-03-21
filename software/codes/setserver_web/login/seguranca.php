<?php
ob_start();


if(($_SESSION['validacao'] == "")){
	unset($_SESSION['validacao']

		//$_SESSION['usuarioId'],			
		//$_SESSION['usuarioNome'], 		
		//$_SESSION['usuarioNivelAcesso'], 
		//$_SESSION['usuarioLogin'], 		
		//$_SESSION['usuarioSenha']
		);
	//Mensagem de Erro
	$_SESSION['loginErro'] = "Área restrita para usuários cadastrados";
	
	//Manda o usuário para a tela de login
	header("Location: index.php");
}