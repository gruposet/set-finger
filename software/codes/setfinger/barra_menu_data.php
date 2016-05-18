    <!-- Inicio navbar -->
    <nav class="navbar  navbar-default navbar-fixed-top"> <!--navbar-inverse-->
      <div class="container">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="index.php"><img src="sf.png" width="45px"/></a>
          
        </div>
        <div id="navbar" class="navbar-collapse collapse">
          <ul class="nav navbar-nav navbar-right">
            <li>
              
            </li>
            <li class="dropdown">
              <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false">Opções <span class="caret"></span></a>
              <ul class="dropdown-menu">
                <li><a href="usuarios_cadastrados.php">Usuários cadastrados</a></li>
                <li role="separator" class="divider"></li>
                <!--<li class="dropdown-header">Nav header</li>-->
                <li><a href="acessos.php">Acessos do dia</a></li>
                <li role="separator" class="divider"></li>
                <li><a href="acessos_data.php">Consulta de acessos por data</a></li>
                <li role="separator" class="divider"></li>
                <li><a href="usuarios_cadastrados.php">Consulta de acessos por usuário</a></li>
                <li role="separator" class="divider"></li>
                <li><a href="novos_usuarios_cadastrados.php">Novos usuários</a></li>
                <li role="separator" class="divider"></li>
                <li><a href="frequencia.php">Frequência</a></li>
                </ul>
            </li>
            <li><a href="sair.php">Sair</a></li>
          </ul>
          

          <!--DATA - Datepicker em Português foram modificados os valores da função fn.datepicker.dates no arquivo bootstrap-datepicker.min.js-->
          </br>
          <div class="container-fluid row"> 
            <form action="" class="form-horizontal" method="post">
              <div class="form-group">
        
                <label class="control-label col-sm-3">Data: </label>
      
                <div class="col-sm-2">
                  <div id="datepicker" class="input-group date" data-date-format="dd-mm-yyyy">
                  <input name="date" class="form-control" type="text" readonly />
                  <span class="input-group-addon"><i class="glyphicon glyphicon-calendar"></i></span>
                  </div>
                </div>
    
                <div class="col-sm-4">
                  <input name="_honey" style="display:none" type="text"/>
                  <button class="btn btn-primary " name="submit" type="submit"> Consultar </button>
                </div>
              </div>   
            </form>
          </div>

          
        </div><!--/.nav-collapse -->
      </div>
    </nav>
<!-- Fim navbar -->
