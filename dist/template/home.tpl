<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="WebChat">
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

      <div class="navbar-form navbar-left" role="search">
        <div class="form-group">
          <input id="search_box" type="text" class="form-control" placeholder="添加好友"/>
        </div>
        <button type="search_button" class="btn btn-default">搜索</button>
      </div>
      <ul class="nav navbar-nav navbar-right" id="nav_personal_center">
        <li class="dropdown">



          <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false">个人中心<span class="badge"></span></a>

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
  <div class="row">
    <div class="col-md-6">
      <div id="latest_chat"> 
          <button id="active">Buttom</button>
          <ul class="list-group" id="table">
          </ul>
      </div>

      <div id="myteam" class="panel-group col-md-6" >      
      </div>
    

      <div id="search_friend" class="row">
        <div class="list-group col-md-6">
          
        </div>
      </div>

      <div id="notification_list" class="row">
        <ul class="list-group col-md-12">
          
        </ul>
      </div>

    </div>
    
    <div class="col-md-6">
      <div id="chat_window">
      </div>
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



    <!--好友请求模态框-->

    <div id="gridSystemModal" class="modal fade" role="dialog" aria-labelledby="gridSystemModalLabel">
       <div class="modal-dialog" role="document">
        <div class="modal-content">
          <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            <h4 class="modal-title" id="gridSystemModalLabel">添加好友</h4>
          </div>
          <div class="modal-body">
            <div class="container-fluid">
              <div class="row">
                <div class="input-group col-md-8 col-md-offset-2">
                  <span class="input-group-addon">用户名</span>
                      <text type="text" class="form-control" id="modal_friend_username">lxyyxl638</text>
                </div>
                </br>
                 <div class="input-group col-md-8 col-md-offset-2">
                  <span class="input-group-addon">昵称</span>
                      <text type="text" class="form-control" id="modal_friend_nickname">lxyyxl638</text>
                </div>
                </br>
                <div class="input-group col-md-8 col-md-offset-2">
                  <span class="input-group-addon">验证请求</span>
                      <input type="text" class="form-control" request-uid="0"/>
                </div>                           
              </div>
            </div>
          </div>
          <div class="modal-footer">
            <button type="button" class="btn btn-default" data-dismiss="modal">关闭</button>
            <button type="button" class="btn btn-primary" button-operation="add_friend">发送请求</button>
          </div>
        </div><!-- /.modal-content -->
      </div><!-- /.modal-dialog -->
    </div><!-- /.modal -->


    <!--好友确认模态框-->
    <div id="gridSystemModalConfirm" class="modal fade" role="dialog" aria-labelledby="gridSystemModalConfirm">
       <div class="modal-dialog" role="document">
        <div class="modal-content">
          <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            <h4 class="modal-title" id="gridSystemModalConfirm">添加好友</h4>
          </div>
          <div class="modal-body">
            <div class="container-fluid">
              <div class="row">
                <div class="input-group col-md-8 col-md-offset-2" id="handle_request">
                  <span class="input-group-addon">接受</span>
                      <input type="radio" checked="checked" name="acceptOrNot" value="接受" />
                  <span class="input-group-addon">拒绝</span>
                      <input type="radio" name="acceptOrNot" value="拒绝" />
                </div>
                <div class="input-group col-md-8 col-md-offset-2">
                  <span class="input-group-addon">分组</span>
                      <select class="form-control" id="ask_teams">
                      </select>                
                </div>                           
              </div>
            </div>
          </div>
          <div class="modal-footer">
            <button type="button" class="btn btn-default" data-dismiss="modal">关闭</button>
            <button type="button" class="btn btn-primary" button-operation="add_friend">发送请求</button>
          </div>
        </div><!-- /.modal-content -->
      </div><!-- /.modal-dialog -->
    </div><!-- /.modal -->

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
