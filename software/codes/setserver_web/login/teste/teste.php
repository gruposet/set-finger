<?php

include_once("conexao.php");

if(isset($_POST['usuario']) && strlen($_POST['usuario']) > 0){

    if (!isset($_SESSION))
         session_start();

    $_SESSION['usuario'] = $mysqli->escape_string($_POST['usuario']);
    $_SESSION['senha'] = md5(md5($_POST['senha']));

    $consultabd = "SELECT id, senha FROM login WHERE usuario = '$_SESSION[usuario]'";
    $result_consultabd = $mysqli->query($consultabd) or die($mysqli->error);
    $resultado = $result_consultabd->fetch_assoc();
    $total_usuarios = $result_consultabd->num_rows;
    
    if($total_usuarios == 0){
        $erro[] = "Usuário não existe!";
    }else{

        if($resultado['senha'] == $_SESSION['senha']){

            $_SESSION['user_id'] = $resultado['id'];
            
        }else{

            $erro[] = "Senha incorreta";

        }
    
    }

    if(count($erro) == 0  || !isset($errro)){

    echo "<script>alert('Login efetuado com sucesso'); location.href='sucesso.php';</script>";

    }
}

?><html>
    <head></head>
    <body>
        <?php if(count($erro) >0)   

            foreach ($erro as $msg) {
                echo "<p>$msg</p> ";
            }
        ?>
        <form method="POST" action="">
            <p><input value="<?php echo $_SESSION['usuario']; ?>" name="usuario" placeholder="email" type="text"></p>
            <p><input name="senha" type="password"></p>
            <p><a href="">esqueceu sua senha?</a>a></p>
            <p><input value="entrar" type="button"></p>
        </form>
    </body>
</html>