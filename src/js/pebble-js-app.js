// ACK/NACK placeholder for debugging
function gotACK(e) {
	//console.log("Received ACK from watch!");
}

function gotNACK(e) {
	//console.log("Received NACK from watch!");
}

// Track Request
function GetLocalTrack() {
    var response;
    var req = new XMLHttpRequest();
    var url = "http://www.abc.net.au/triplej/feeds/playout/triplej_brisbane_playout_history.xml";

    req.open('GET', url, true);
    req.onload = function(e) {
        if (req.readyState == 4 && req.status == 200) {
			if (req.status == 200) {
				//var track_pattern = />Temperature<\/a>[\s\S]*?(\d+\.\d).C/gm;
				var track_pattern = /<title>(.+?)<.title[\s\S]*?<artistname>(.+?)<.artistname/gm;
				var track_obj = track_pattern.exec(req.responseText);

				if (track_obj != null) {
					console.log("Matched: " + track_obj[2] + " - " + track_obj[1]);

					Pebble.sendAppMessage({
			        	"localTrack" : decodeURI(track_obj[1]),
			        	"localArtist" : decodeURI(track_obj[2]),
					}, gotACK, gotNACK);
					console.log("Local track sent!");
				} else {
					console.log("JPLAY regex failed");
				}
			} else { 
				console.log("Error"); 
			}
        }
    }
    req.send(null);
}

// Track Request
function GetLiveTrack() {
    var response;
    var req = new XMLHttpRequest();
    var url = "http://www.abc.net.au/triplej/feeds/playout/triplej_sydney_3item_playout.xml";

    req.open('GET', url, true);
    req.onload = function(e) {
        if (req.readyState == 4 && req.status == 200) {
			if (req.status == 200) {
				//var track_pattern = />Temperature<\/a>[\s\S]*?(\d+\.\d).C/gm;
				var track_pattern = /<title>(.+?)<.title[\s\S]*?<artistname>(.+?)<.artistname/gm;
				var track_obj = track_pattern.exec(req.responseText);

				if (track_obj != null) {
					console.log("Matched: " + track_obj[2] + " - " + track_obj[1]);

					Pebble.sendAppMessage({
			        	"liveTrack" : decodeURI(track_obj[1]),
			        	"liveArtist" : decodeURI(track_obj[2]),
					}, gotACK, gotNACK);
					console.log("Live track sent!");

					// GET LOCAL TRACK
					GetLocalTrack();

				} else {
					console.log("JPLAY regex failed");
				}
			} else { 
				console.log("Error"); 
			}
        }
    }
    req.send(null);
}


Pebble.addEventListener("ready", function(e) {
    	// Send an initial message to the Pebble
		Pebble.sendAppMessage({
			"jsLoaded" : 1,
		}, gotACK, gotNACK);

        console.log("JPlay JS Loaded");
    }
);

Pebble.addEventListener("appmessage", function(e) {
    //console.log("Received message: " + e.payload["requestBTC"]);

    if (e.payload["requestTracks"]) {
    	// AppMessage requested new BTC price
    	console.log("Recieved AppMessage request for new track info");
    	GetLiveTrack();
    }

    
  }
);
