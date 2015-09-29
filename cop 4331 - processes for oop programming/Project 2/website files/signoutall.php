<?
	session_start(); 
		 $BASEDIR=$_SERVER['DOCUMENT_ROOT']."/";
		 INCLUDE $BASEDIR."main.php";
		 INCLUDE $BASEDIR."sqldb.php";
		 main::sqldb();
		 $db=main::$db;
		 $db->q("update users set online=0 "); 
		
	session_destroy();
	header("Location:/");
?>