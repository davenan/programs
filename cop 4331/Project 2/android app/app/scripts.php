<script>
	var lastid=0,recipient=0;
	$("#sndbutton").click(function(){
		if ($.trim($('#newmessage').val())!=""){ 
			$.post("/app/index.php",{
				sendMessage:$("#newmessage").val(),
				recipient:recipient
			},function(data){ 
				console.log(data);
				$("#messageHolder").append('<div><p class="outgoing">You:' + $("#newmessage").val()+'</p></div>');
				$("#newmessage").val("");
				$("#messageHolder").scrollTop($("#messageHolder").prop("scrollHeight"));
			});
		};
	});
	function getMessages(){
		$.ajax({
		  type: "POST",
		  url: "/app/index.php",
		  data: {
					getMessage:recipient,
					lastView:lastid
				},
		  success: function(data){  
						if($.trim(data.html)!="")
						{  
							$("#messageHolder").append(data.html);
							$("#messageHolder").scrollTop($("#messageHolder").prop("scrollHeight"));
						}
						if(lastid==0)
							setTimeout(function(){$("#messageHolder").animate({scrollTop:$("#messageHolder").prop("scrollHeight")});}, 100);
						lastid=data.lastid; 
						setTimeout(function(){getMessages();}, 500);
					},
		  dataType: "json"
		}); 
	}
	function getStatus(){
		$.ajax({
		  type: "POST",
		  url: "/app/index.php",
		  data: {
					getStatus:"m"
				},
		  success: function(data){ 
        			  var justsignin=""; 
					for(var i=0;i<data.total;i++){ 
						$("#"+data.status[i].id+" img").attr('src',data.status[i].status+'.png');
						if ($("#"+data.status[i].id+" img").attr('src')!=(data.status[i].status+'.png'))
							justsignin+=data.status[i].name+"<br/>";
					}
					if($.trim(justsignin)!=""){
						$("#justsignin").html(justsignin+" just sign in");
						$("#justsignin").show();
						$("#justsignin").fadeOut(10000); 
					}
					getStatus();
				},
		  dataType: "json"
		});
	}
	getStatus(); 
	function resizeMessageHolder(){
		var height=$(window).height()*.97;
		$("#messageHolder").height(height-50-(height*.02));
	}
	resizeMessageHolder();
	$( window ).resize(function() {
	  resizeMessageHolder();
	});
	$(".userList").click(function(){
		recipient=$(this).attr("id"); 
		$("body").animate({scrollLeft:$("#chat").prop("scrollWidth")});
		lastid=0; 
		$("#messageHolder").html("");
		getMessages();
	});
	function getLocations(){
		console.log("hey");
		$.ajax({
		  type: "POST",
		  url: "/inbox.php",
		  data: {
					getLocations:"m"
				},
		  success: function(data){  
		  console.log(data.total);
					for(var i=0;i<data.total;i++){ 
						if(isNaN(data.status[i].lat)|| isNaN(data.status[i].lon)){
							$("#"+data.status[i].id+" span").html("<br>Location unavailable.");
						}
						else
						$("#"+data.status[i].id+" span").html("<br>Distance from you: "+
										(google.maps.geometry.spherical.computeDistanceBetween(
										currentPosition, new google.maps.LatLng(data.status[i].lat, data.status[i].lon)) ).toFixed(2)+" meters"); 
					} 
					setTimeout(function(){ getLocations(); }, 60000);
				},
		  dataType: "json"
		});
	}
		getLocations();
</script> 