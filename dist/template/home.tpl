<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="光彪楼健身房">
    <meta name="author" content="Carson">

    <title>WebChat</title>

    <!-- Bootstrap core CSS -->
    <link href="../dist/css/bootstrap.min.css" rel="stylesheet">
    <link rel='stylesheet' href='../dist/css/chat.css' type='text/css' media='all' />

    <!-- <style type="text/css">
     /* body{
            background:url('../img/home.jpg');
            background-size:100% 100%;
            -moz-background-size:100% 100%; /* 老版本的 Firefox */
            background-repeat:no-repeat;
          }*/
    </style> -->
  </head>
<!-- NAVBAR
================================================== -->
  <body>
    <!-- Carousel
    ================================================== -->
    <div class="container-fluid">
      

      <nav class="navbar navbar-default">
        <div class="container-fluid">
          <!-- Brand and toggle get grouped for better mobile display -->
          <div class="navbar-header">
          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1" aria-expanded="false">
              <span class="sr-only">Toggle navigation</span>
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
              <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="#">WebChat</a>
       </div>

    <!-- Collect the nav links, forms, and other content for toggling -->
    <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
      <ul class="nav navbar-nav nav-tabs">
        <li class="active"><a href="#latest_chat" data-toggle="tab">最近会话 <span class="sr-only">(current)</span></a></li>
        <li><a href="#myteam" id="nav_team" data-toggle="tab">我的好友</a></li>
      </ul>

      <form class="navbar-form navbar-left" role="search">
        <div class="form-group">
          <input type="text" class="form-control" placeholder="添加好友">
        </div>
        <button type="submit" class="btn btn-default">搜索</button>
      </form>
      <ul class="nav navbar-nav navbar-right">
        <li class="dropdown">
          <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false">个人中心 <span class="caret"></span></a>
          <ul class="dropdown-menu">
            <li><a href="#">修改资料</a></li>
            <li><a href="#">Another action</a></li>
            <li><a href="#">Something else here</a></li>
            <li role="separator" class="divider"></li>
            <li><a href="#">退出</a></li>
          </ul>
        </li>
      </ul>
    </div><!-- /.navbar-collapse -->
  </div><!-- /.container-fluid -->
</nav>

    <div id="latest_chat" class="row"> 
        <div class="col-md-6"> 
          <button id="active">Buttom</button>
          <ul class="list-group" id="table">
          </ul>
        </div>
        

        <div id="chat_window" class="col-md-6">
        </div>
    </div>

    <div id="myteam" class="panel-group col-md-6" >
        
    </div>
    

    <div id="search_friend" class="row">
      <div class="list-group col-md-6">
         <button type="button" class="list-group-item">Cras justo odio</button>
        <button type="button" class="list-group-item">Dapibus ac facilisis in</button>
  <button type="button" class="list-group-item">Morbi leo risus</button>
  <button type="button" class="list-group-item">Porta ac consectetur ac</button>
  <button type="button" class="list-group-item">Vestibulum at eros</button>
</div>
    </div>
  </div>
    <footer class="footer">
      <div class="row footer-bottom">
        <ul class="list-inline text-center">
          <li></li>
        </ul>
      </div>
    </footer>
    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->

    <script src="../dist/js/jquery-2.1.1.min.js"></script>
    <script src="../dist/js/bootstrap.min.js"></script>
    <script src="../dist/js/home.js"></script>
    <script src="../dist/js/json2.js"></script>
    <script src="../dist/js/icomet.js"></script>
    <script src="../dist/js/chat.js"></script>
  </body>
</html>
