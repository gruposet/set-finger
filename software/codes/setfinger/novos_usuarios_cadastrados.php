<?php
session_start();
include_once("seguranca.php");
include_once("conexao.php");

if(isset($_SESSION['usuarioNiveisAcesso'])){
      if ($_SESSION['usuarioNiveisAcesso'] == "0") {
          header("Location: colaborador.php");              
      }
  }
?>




<!DOCTYPE html>
<html lang="pt-br">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
    <meta name="description" content="Página de log Setserver">
    <meta name="author" content="">
    <link rel="icon" href="sf.png">

    <title>Setserver admin</title>

    <!-- Bootstrap core CSS -->
    <link href="css/bootstrap.min.css" rel="stylesheet">
    <!-- Bootstrap theme -->
    <link href="css/bootstrap-theme.min.css" rel="stylesheet">
    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <link href="css/ie10-viewport-bug-workaround.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="css/theme.css" rel="stylesheet">

    <!-- Just for debugging purposes. Don't actually copy these 2 lines! -->
    <!--[if lt IE 9]><script src="../../assets/js/ie8-responsive-file-warning.js"></script><![endif]-->
    <script src="js/ie-emulation-modes-warning.js"></script>

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
  </head>

  <body role="document">

  <?php
      include_once("barra_menu_admin.php");

      $selecao_usuarios= "SELECT * FROM login ORDER BY nome";
      $consulta_usuarios= mysqli_query($conn, $selecao_usuarios);
      $linhas=$consulta_usuarios->num_rows;
  ?>

    <div class="container theme-showcase" role="main">

      <div class="page-header text-center">
        <h1>Usuários cadastrados</h1>
      </div>

        <div class="table-responsive"> <!--col-md-6-->
          <table class="table table-striped">
            <thead>
              <tr>
                <th>DIN</th>
                <th>Nome</th>
                <th>E-mail</th>
                <th>Nivel de acesso</th>
              </tr>
            </thead>
            <tbody>
            <?php
                while($linhas = $consulta_usuarios->fetch_array()){
                  echo "<tr>";
                  echo "<td>".$linhas['id']."</td>";
                  echo "<td>".$linhas['nome']."</td>";
                  echo "<td>".$linhas['email']."</td>";
                  echo "<td>".$linhas['nivel_acesso']."</td>";
                  echo "<tr>";
                }
            ?>
            </tbody>
          </table>
        </div>
      </div>

    </div> <!-- /container -->


    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
    <script>window.jQuery || document.write('<script src="js/vendor/jquery.min.js"><\/script>')</script>
    <script src="js/bootstrap.min.js"></script>
    <script src="js/docs.min.js"></script>
    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <script src="js/ie10-viewport-bug-workaround.js"></script>
  </body>
</html>
