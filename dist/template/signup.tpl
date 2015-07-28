




<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>Web Chat</title>
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
    <div style="position:absolute;top: 40%;left: 40%;width: 15%;height: auto">
      <form action="http://localhost/post.cgi" method="post">
	<div class="form-group">
            <div class="row">
                <div class="col-md-12">
                <input type="text" name="username" class="form-control"  placeholder="用户名">
                </div>
            </div>
        </div>
        <div class="form-group">
            <input type="password" name="password" class="form-control" placeholder="密码">
        </div>

        <div class="form-group">
            <input type="text" name="nickname" class="form-control"placeholder="昵称">
        </div>
        <div class="form-group">
                <select class="form-control" name="sex">
                    <option value="male">男性</option>
                    <option value="female">女性</option>
                </select>
        </div>
        <div class="row">
            <div class="col-md-12">
                <button type="submit" class="btn btn-danger" style="width: 100%;">注册</button>
            </div>
        </div>
    </form>
</div>

</body>
</html>
