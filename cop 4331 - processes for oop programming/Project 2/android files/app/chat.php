<style> 
	#sndbutton{
		background:#18bc9c;
		font-size:16px;
		border-radius:12px;
		border:none;
		width:90%;
		color:#FFF;
		height:75%;
		border:
	}
	#messageHolder{
		width:100%;
		padding-right:20px;
		height:90%;
		background:#FFF;
		overflow-y:scroll;
	}
	#messageHolder div{
		overflow:hidden;
		width:100%;
		margin-bottom:1px;
	}
	.incoming{
		color:#000;
		background:#A8A8A8;
		float:left; 
		padding:10px;
		border-radius:10px;
		font-size:20px;
		max-width:80%;
		margin-left:20px;
	}
	.outgoing{
		color:#FFF;
		background:#13967D;
		float:right;
		padding:10px;
		border-radius:10px;
		font-size:20px;
		max-width:80%;
	}
</style> 
<div style="width:97%;height:100%;margin:auto;margin-top:1.5%;overflow:hidden">
	<div id="messageHolder"> 
	</div>
	<table style="width:100%;height:50px;margin-top:2%;border-collapse:collapse;">
		<tr>
			<td style="width:75%;height:100%;">
				<textarea id="newmessage" maxlength="100" placeholder="Enter message here" style="width:100%;height:100%;border:none"></textarea>
			</td>
			<td style="width:25%;height:100%;text-align:center;"><button id="sndbutton">Send</button></td>
		</tr>
	</table>
</div>
