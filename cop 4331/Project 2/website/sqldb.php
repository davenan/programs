<?php
	// SQLDB Wrapper
	// Written by Michael Rogers
	// Copyright 2013-2014
	
	// Database wrapper
	class sqldb{
		private $db; // Database object holder
	
		function __construct($host,$user,$pass,$db){ // Build the database connection
			$this->db=new mysqli($host,$user,$pass,$db); // Create connection
			if($this->db->connect_errno){throw new Exception("Unable to connect to database: ".$this->db->connect_error);} // If error, throw it
		}
		
		// Run a query (when num is false, returns object instead of 0 for empty set)
		function q($query,$num=true){
			if($r=$this->db->query($query)){ // Return a result (r) for the query, and if valid
				if($r===true){return $this->db->affected_rows;} // For insert/update/delete statements, return affected row count
				elseif(!$r->num_rows&&$num){return 0;} // For empty set while num is true, return 0
				else{return new sqldb_result($r);} // Otherwise, return the result
			}else{return false;} // If invalid, return false for error
		}
		
		// Prepare a statement
		function p($query){
			if($s=$this->db->prepare($query)){ // Create the statement, and if valid
				return new sqldb_stmt($s); // Return the statement
			}else{return false;} // If invalid, return false for error
		}
		
		// Return the error
		function error(){return $this->db->error;}
		
		// Return the last insert id
		function id(){return $this->db->insert_id;}
		
		// Return info on last query
		function info(){return $this->db->info;}
		
		// Return the injection safe string
		function safe($str){return $this->db->real_escape_string($str);}
		
		// Return the database handler
		function sql(){return $this->db;}
	}
	
	// Result wrapper
	class sqldb_result{
		private $r; // Request object holder
	
		// Absorb the result
		function __construct($r){$this->r=$r;}
		
		// Get the row as an associative array, or numeric if x is true. If x is a number it will act as a seek, and y becomes the the numeric toggle
		function row($x=false,$y=false){
			if(is_numeric($x)){$this->seek($x);}
			if($x===true||$y===true){return $this->r->fetch_row();}
			else{return $this->r->fetch_assoc();}
		}
		
		// Get the number of rows
		function num(){return $this->r->num_rows;}
		
		// Get the fields as an array
		function fields(){return $this->r->fetch_fields;}
		
		// Jump to specified row
		function seek($n=0){return $this->r->data_seek($n);}
		
		// Return the result handler
		function result(){return $this->r;}
	}
	
	// Statement wrapper
	class sqldb_stmt{
		private $s; // Statement object holder
	
		// Absorb the statement
		function __construct($s){$this->s=$s;}
		
		// Return the error
		function error(){return $this->s->error;}
		
		// Return the last insert id
		function id(){return $this->s->insert_id;}
		
		// Return result metadata
		function meta(){return $this->s->result_metadata();}
		
		// Get the number of rows
		function num(){return $this->s->num_rows;}
		
		// Get the number of parameters
		function numParams(){return $this->s->param_count;}
		
		// Get the number of parameters
		function numFields(){return $this->s->field_count;}
		
		// Jump to specified row
		function seek($n=0){return $this->s->data_seek($n);}
		
		// Bind parameters
		//function params($str,$arr){return call_user_func_array(array($this->s,"bind_param"),refVal($arr,$str));}
		
		// Bind results
		//function results($arr){return call_user_func_array(array($this->s,"bind_result"),refVal($arr));}
		
		// Fetch results into bound variables
		function fetch($type){return $this->s->fetch();}
		
		// Execute a query
		function execute($type){return $this->s->execute();}
		
		// Get the result object
		function r(){
			if($r=$this->s->get_result()){ // Return a result (r) for the query, and if valid
				if($r===true){return $this->s->affected_rows;} // For insert/update/delete statements, return affected row count
				elseif(!$r->num_rows&&$num){return 0;} // For empty set while num is true, return 0
				else{return new sqldb_result($r);} // Otherwise, return the result
			}else{return false;} // If invalid, return false for error
		}
		
		// Return the statement handler
		function stmt(){return $this->s;}
		
		// Get referential values
		private function refVal($arr,$str=null){
			$refs=array();
			foreach($arr as $k){$refs[$k]=&$arr[$k];}
			if($str){array_unshift($str);}
			return $refs;
		}
	}
?>