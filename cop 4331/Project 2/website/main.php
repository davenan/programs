<?php 
   class main{
		static $db;  
		
		// Open the db connection
		static function sqldb(){ 
			return self::$db = new sqldb("localhost","leternel_ucf","ucf@12","leternel_software");
		}
		 
		
		// Change of base for arbitrary length strings (value,from base{2-100},to base{2-100}) (max of 95 recommended, or at worst 98)
		static function rebase($v,$f,$t){
			if($f<2||$f>100||$t<2||$t>100){return false;} // Limit the range
			$c="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/= _-.,@?!&*$#%:;^'()[]{}<>~|`\"\\\t\r\n\v\f"; // Character set
			$v=(string)$v;$b="";$m=strlen($v);for($i=0;$i<$m;$i++){$b=bcadd($b,bcmul(strpos($c,$v[$i]),bcpow($f,$m-1-$i)));} // Convert from starting base to base 10
			$v="";while($b>0){$v=$c[bcmod($b,$t)].$v;$b=bcdiv($b,$t,0);}return $v; // Convert from base 10 to finishing base
		}
		
		// Takes numeric array, returns string
		static function basea_encode($arr){
			$delimiter="a";$str="";$m=count($arr);
			for($i=0;$i<$m;$i++){$str.=($i?$delimiter:"").$arr[$i];}
			return self::rebase($str,11,62);
		}
		
		// Takes string, returns numeric array
		static function basea_decode($str){
			$delimiter="a";$str=self::rebase($str,62,11);
			if(is_numeric($str)){return array($str);}
			elseif(preg_match("/^([\d]+)((".$delimiter.")([\d]+))+$/",$str)){return explode($delimiter,$str);}
			return array();
		}
		
		// Format phone number with symbols
		static function format_phone($phone){
			return preg_replace("/([0-9]{3})([0-9]{3})([0-9]{4})/","($1) $2-$3",$phone); // Add symbols
		}
		
		// Deformat phone number
		static function deformat_phone($phone){
			return preg_replace("/([^0-9])*/","",$phone); // String symbols
		}
		
		// Hash password
		static function passhash($pass){
			return hash("sha256", sha1($pass)); // We use a sha256 over a sha1. Helps secure against decryption databases
		} 
	}
?>