<?
	session_start();
	if(isset($_SESSION['id'])){
		 $BASEDIR=$_SERVER['DOCUMENT_ROOT']."/";
		 INCLUDE $BASEDIR."main.php";
		 INCLUDE $BASEDIR."sqldb.php";
		 main::sqldb();
		 $db=main::$db;
		 $db->q("update users set online=0 where id=".$_SESSION['id']); 
		 
	}
		
	session_destroy();
	header("Location:/");
?>