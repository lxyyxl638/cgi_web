<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title></title>
    <!-- 新 Bootstrap 核心 CSS 文件 -->
    <link rel="stylesheet" href="//cdn.bootcss.com/bootstrap/3.3.5/css/bootstrap.min.css">
    <link rel="stylesheet" href="./dist/css/style.css">

    <!-- jQuery文件。务必在bootstrap.min.js 之前引入 -->
    <script src="//cdn.bootcss.com/jquery/1.11.3/jquery.min.js"></script>
    <!-- 最新的 Bootstrap 核心 JavaScript 文件 -->
    <script src="//cdn.bootcss.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>
</head>
<body>
<div id="background" style="position:absolute;z-index:-1;width:100%;height:100%;top:0px;left:0px;">
    <img src="./dist/img/home.jpg" width="100%" height="100%"/>
</div>
<div class="container">

    <div style="position:absolute;top: 40%;left: 40%;width: 20%;height: auto">
        <div class="row">
		<div class="input-group">
  			<span class="input-group-addon" id="basic-addon1">用户名</span>
  			<input id="username" type="text" class="form-control" placeholder="用户名" aria-describedby="basic-addon1">
		</div>
        </div>

        <div class="row">
            <div class="col-md-12"></div>
            <div class="col-md-12"></div>
            <div class="col-md-12"></div>
            <div class="col-md-12"></div>
        </div>
	<div class="row">
		<div class="input-group">
  			<span class="input-group-addon" id="basic-addon1">密码</span>
  			<input id="password" type="password" class="form-control" placeholder="密码" aria-describedby="basic-addon1">
		</div>
        </div>

        <div class="row">
            <div class="col-md-12"></div>
            <div class="col-md-12"></div>
            <div class="col-md-12"></div>
            <div class="col-md-12"></div>
        </div>
        <div class="row">
	   	<div id="result" style="color:red">
		</div>
                <button id="login" type="buttom" class="btn btn-danger" style="width: 100%;">登录</button>
	</div>

        <div class="row">
            <div class="col-md-12"></div>
            <div class="col-md-12"></div>
            <div class="col-md-12"></div>
            <div class="col-md-12"></div>
        </div>
        <div class="row">
                <button type="button" class="btn btn-success" style="width: 100%" onclick="location.href='/sign_up'">注册</button>
            
        </div>

    </div>


</div>
<script src="./dist/js/sign.js"></script>
</body>
</html>
