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

      
    <script src="js/ie-emulation-modes-warning.js"></script>

    <script type="text/javascript" src="http://code.jquery.com/jquery-1.9.1.js"></script>
  
   

  </head>

  <body role="document">

  <?php
      include_once("barra_menu_admin.php");

      $selecao_usuarios= "SELECT * FROM users ORDER BY name";
      $consulta_usuarios= mysqli_query($conn, $selecao_usuarios);
      $linhas=$consulta_usuarios->num_rows;

      $nome="Frequência do Usuário";
      $horas=array(8, 9, 6, 7, 9);
      $usuario="Frequência do Sr(a) Jefferson Ferreira";
  ?>

   <script type='text/javascript'>

      var user = {text: "<?php print $usuario?>"}

      var js = <?php print json_encode($horas) ?>;

      var dados = [{
        name: "Frequência esperada;",
        data: [4, 4, 4, 4, 4]
      },{
        name: "<?php print $nome; ?>;",
        data: js
      }]
    </script>




    </br>
    <div class="container-fluid row"> 
        <form action="" class="form-horizontal" method="post">
          <div class="form-group">
            
            <div class="col-sm-1 col-xs-offset-2">
            <input name="id" class="form-control" id="user_id" type="text" placeholder="ID">
            </div>

            <label class="control-label col-md-1">De: </label>
  
            <div class="col-sm-1">
              <div id="datepicker" class="input-group date" data-date-format="dd-mm-yyyy">
              <input name="date1" class="form-control" type="text" readonly />
              <span class="input-group-addon"><i class="glyphicon glyphicon-calendar"></i></span>
              </div>
            </div>

            <label class="control-label col-md-2">até: </label>
            <div class="col-sm-2">
              <div id="datepicker2" class="input-group date" data-date-format="dd-mm-yyyy">
              <input name="date2" class="form-control" type="text" readonly />
              <span class="input-group-addon"><i class="glyphicon glyphicon-calendar"></i></span>
              </div>
            </div>

            <div class="col-sm-2">
              <input name="_honey" style="display:none" type="text"/>
              <button class="btn btn-primary " name="submit" type="submit"> Consultar </button>
            </div>
          </div>   
        </form>
    </div>

<?php if(isset($_POST['date1']))
{echo $_POST['date1'];
echo "</br>".$_POST['date2'];
echo "</br>".$_POST['id'];
}
;?>


    <div class="container">
        
        <div class="page-header text-center">
            <h1>Frequência GT-SET</h1>
        </div>

        <div class="row">
          <div class="col-md-12 col-sd-offset-1">
            <div id="container" style="min-width: 310px; height: 400px; margin: 0 auto">
                <script type='text/javascript' src="js/chart_frequencia.js">//<![CDATA[</script>
            </div>
          </div>
        </div>
    </div>
    </br>
    </br>
    </br>
    </br>
    <div class="container theme-showcase" role="main">

        <div class="table-responsive"> <!--col-md-6-->
          <table class="table table-striped">
            <thead>
              <tr>
                <th></th>
                <th>Segunda</th>
                <th>Terça</th>
                <th>Quarta</th>
                <th>Quinta</th>
                <th>Sexta</th>
              </tr>
            </thead>
            <tbody>

            <tr>
            <th>Carga-Horária</br>esperada</th>
            <td>4</td>
            <td>4</td>
            <td>4</td>
            <td>4</td>
            <td>4</td>
            </tr>

            <tr>
            <th>Carga-horária</br>cumprida</th>
            <?php
              echo "<td>".$horas[0]."</td>";
              echo "<td>".$horas[1]."</td>";
              echo "<td>".$horas[2]."</td>";
              echo "<td>".$horas[3]."</td>";
              echo "<td>".$horas[4]."</td>";
            ?>            
            </tr>
              
            </tbody>
          </table>
        </div>
    </div>


 </body>
          




    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    
    <script src="js/bootstrap.min.js"></script>
    <script src="js/docs.min.js"></script>
    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <script src="js/ie10-viewport-bug-workaround.js"></script>
  </body>

  <script src="js/highcharts.js"></script>
<script src="https://code.highcharts.com/modules/exporting.js"></script>

<!--Datepicker-->
    <script src='js/bootstrap-datepicker.js'></script>
    <script src="js/index.js"></script>

    <!--Datepicker-->
    <script src='js/bootstrap-datepicker.js'></script>
    <script src="js/index.js"></script>

</html>
