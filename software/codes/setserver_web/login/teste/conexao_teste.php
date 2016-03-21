<?php
$conectar = mysqli_connect("localhost","root","") or die ("Erro na conexão");
mysqli_select_db($conectar,'setserver_admin') or die ("BD não encontrado");


$result = mysqli_query($conectar,"SELECT * FROM usuarios WHERE usuario='$ler_usuario' AND senha='$ler_senha' LIMIT 1");
$resultado = mysqli_fetch_assoc($result);
echo "usuario: ".$resultado['nome'];
?>