<?php

session_start();
$ler_usuario = $_POST['usuario'];
$ler_senha = $_POST['senha'];

include_once("conexao.php");
#Outra forma de conexão do bd
#$conectar = mysqli_connect("localhost","root","") or die ("Erro na conexão");
#mysqli_select_db($conectar,'setserver_admin') or die ("BD não encontrado");


################################## CONSULTA BD ##################################
#especificação da consulta no bd
$consultabd = "SELECT * FROM login WHERE usuario='$ler_usuario' AND senha='$ler_senha' LIMIT 1";

#executa a consulta no bd, especificada na variável $consultabd, e recebe o resultado da consulta
$result_consultabd = $mysqli->query($consultabd) or die($mysqli->error);

#Outra forma de fazer consulta no bd
#$result = mysqli_query($mysqli,"SELECT * FROM login WHERE usuario='$ler_usuario' AND senha='$ler_senha' LIMIT 2");

#captura o resultado da consulta no bd
$resultado = $result_consultabd->fetch_assoc();

#Outra forma de capturar o resultado da consulta no bd
#$resultado = mysqli_fetch_assoc($result_consultabd);


echo nl2br ("usuario: ".$resultado['nome']."\n email: ".$resultado['email']."\n id: ".$resultado['id']);



?> 

<html>
	<head>
		<meta charset="utf8">
	<head>
	<body>
		<table border="1">
			<tr>
				<td>id</td>
				<td>nome</td>
				<td>email</td>
			</tr>
			<?php while($dado = $result_consultabd->fetch_array()){ ?>
			<tr>
				<td><?php echo $dado["id"]; ?></td>
				<td><?php echo $dado["nome"]; ?></td>
				<td><?php echo $dado["email"]; ?></td>
			</tr>
			<?php } ?>
		</table>
	<body>