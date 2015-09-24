<style> 
	.userList{
		width:100%; 
		padding-left:5px;
		border-top:2px solid #FFF;
		border-bottom:2px solid #FFF;
		position:relative;
		color:#FFF;
		height:70px;
		background:#2c3e50;
	}
	.userList span{
		padding:0;
	}
	.userList p{
		
		font-size:20px;
		padding:0;
		margin:0;
	}
	.userList img{
		height:40px;
		position:absolute;
		top:5px;
		right:5px;
	}
	.online{
			/* Permalink - use to edit and share this gradient: http://colorzilla.com/gradient-editor/#8fc400+0,8fc400+100;Green+Flat+%236 */
			background: #8fc400; /* Old browsers */
			/* IE9 SVG, needs conditional override of 'filter' to 'none' */
			background: url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiA/Pgo8c3ZnIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIgd2lkdGg9IjEwMCUiIGhlaWdodD0iMTAwJSIgdmlld0JveD0iMCAwIDEgMSIgcHJlc2VydmVBc3BlY3RSYXRpbz0ibm9uZSI+CiAgPGxpbmVhckdyYWRpZW50IGlkPSJncmFkLXVjZ2ctZ2VuZXJhdGVkIiBncmFkaWVudFVuaXRzPSJ1c2VyU3BhY2VPblVzZSIgeDE9IjAlIiB5MT0iMCUiIHgyPSIwJSIgeTI9IjEwMCUiPgogICAgPHN0b3Agb2Zmc2V0PSIwJSIgc3RvcC1jb2xvcj0iIzhmYzQwMCIgc3RvcC1vcGFjaXR5PSIxIi8+CiAgICA8c3RvcCBvZmZzZXQ9IjEwMCUiIHN0b3AtY29sb3I9IiM4ZmM0MDAiIHN0b3Atb3BhY2l0eT0iMSIvPgogIDwvbGluZWFyR3JhZGllbnQ+CiAgPHJlY3QgeD0iMCIgeT0iMCIgd2lkdGg9IjEiIGhlaWdodD0iMSIgZmlsbD0idXJsKCNncmFkLXVjZ2ctZ2VuZXJhdGVkKSIgLz4KPC9zdmc+);
			background: -moz-linear-gradient(top,  #8fc400 0%, #8fc400 100%); /* FF3.6+ */
			background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,#8fc400), color-stop(100%,#8fc400)); /* Chrome,Safari4+ */
			background: -webkit-linear-gradient(top,  #8fc400 0%,#8fc400 100%); /* Chrome10+,Safari5.1+ */
			background: -o-linear-gradient(top,  #8fc400 0%,#8fc400 100%); /* Opera 11.10+ */
			background: -ms-linear-gradient(top,  #8fc400 0%,#8fc400 100%); /* IE10+ */
			background: linear-gradient(to bottom,  #8fc400 0%,#8fc400 100%); /* W3C */
			filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#8fc400', endColorstr='#8fc400',GradientType=0 ); /* IE6-8 */
			cursor:pointer;
	}

	.offline{
		   /* Permalink - use to edit and share this gradient: http://colorzilla.com/gradient-editor/#cc0000+0,cc0000+100;Red+Flat */
	background: #cc0000; /* Old browsers */
	background: -moz-linear-gradient(top,  #cc0000 0%, #cc0000 100%); /* FF3.6+ */
	background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,#cc0000), color-stop(100%,#cc0000)); /* Chrome,Safari4+ */
	background: -webkit-linear-gradient(top,  #cc0000 0%,#cc0000 100%); /* Chrome10+,Safari5.1+ */
	background: -o-linear-gradient(top,  #cc0000 0%,#cc0000 100%); /* Opera 11.10+ */
	background: -ms-linear-gradient(top,  #cc0000 0%,#cc0000 100%); /* IE10+ */
	background: linear-gradient(to bottom,  #cc0000 0%,#cc0000 100%); /* W3C */
	filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#cc0000', endColorstr='#cc0000',GradientType=0 ); /* IE6-9 */
	}
</style> 
<div>
	<?
		$query="select distinct u.id id,
				       u.user_name name
				FROM friends fr 
				INNER JOIN users u on u.id=fr.friendsuserid
				where fr.userid=".$_SESSION['id'];
					
		if($r=$db->q($query))
			while($row=$r->row())
			{?>
			<div  class="userList" id="<?=main::basea_encode(array(1,$row['id'],1));?>">
				<p><?=$row['name'];?></p>
				<span></span>
				<img src="offline.png" />
			</div>	
		<?	}
	?>
		
</div> 