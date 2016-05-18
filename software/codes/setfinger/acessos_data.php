<?php
session_start();
include_once("seguranca.php");
include_once("conexao.php");

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

    <!--Datepicker-->
    <link rel='stylesheet prefetch' href='http://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css'>
    <link rel='stylesheet prefetch' href='http://cdnjs.cloudflare.com/ajax/libs/bootstrap-datepicker/1.3.0/css/datepicker.css'>
    <link rel="stylesheet" href="css/style.css">



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
      include_once("barra_menu_data.php");

      if(isset($_POST['date'])){

      $data_label=$_POST['date'];
      }
      else{$data_label="...";}

  ?>

    <div class="container theme-showcase" role="main">

      <div class="page-header text-center">
        <?php echo "<h1>Registros de acesso de usuários em ".$data_label."</h1>";
        ?>
      </div>

        <div class="table-responsive"> <!--col-md-6-->
          <table class="table table-striped">
            <thead>
              <tr>
                <th>DIN</th>
                <th>Nome</th>
                <th>Data</th>
                <th>Hora</th>
              </tr>
            </thead>
            <tbody>
            <?php
                if(isset($_POST['date'])){
        
                  $data_i=strtotime($_POST['date']);
                  $data_f=strtotime($_POST['date']."23:59:59");
                  
                  $selecao_usuarios= "SELECT * FROM history WHERE timestamp BETWEEN '$data_i' AND '$data_f' ";
                  $consulta_usuarios= mysqli_query($conn, $selecao_usuarios);
                  $resultado = mysqli_fetch_assoc($consulta_usuarios);      
                  

                  if(isset($resultado)){
                    $consulta_usuarios= mysqli_query($conn, $selecao_usuarios);
                    $linhas=$consulta_usuarios->num_rows;
      
                    while($linhas = $consulta_usuarios->fetch_array()){
                          echo "<tr>";
                          echo "<td>".$linhas['din']."</td>";
                          echo "<td>".$linhas['name']."</td>";
                          echo "<td>".date('m/d/Y', $linhas['timestamp'])."</td>";
                          echo "<td>".date('H:i', $linhas['timestamp'])."</td>";
                          echo "<tr>";
                    }
                  }

                  else{
                      echo '<p class="text-center text-danger">';
                      echo 'Não há registros de acesso para essa data';
                      echo '</p>';
                      }
                }         
                
                unset($_POST['date']);
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
    <script src='http://cdnjs.cloudflare.com/ajax/libs/bootstrap-datepicker/1.3.0/js/bootstrap-datepicker.js'></script>
    <script src="js/index.js"></script>

    <!--Datepicker-->
    <script src='js/bootstrap-datepicker.js'></script>
    <script src="js/index.js"></script>


  </body>
</html>
