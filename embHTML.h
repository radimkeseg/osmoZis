#include "Arduino.h" //not sure why but here it is needed

const char PAGE_INDEX[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>Electro-osmosis</title>


  <style>
.rGauge {
  width: 200px;
  height: 145px;
}
.rGauge-base {
  stroke: #edebeb;
  fill: none;
}
.rGauge-progress {
  fill: none;
  transition: 0.5s;
}
.rGauge-val {
  font-weight: 700;
  font-size: 1.3em;
}
.rGauge-min-val, .rGauge-max-val {
  fill: #b3b3b3;
}
.vGauge {
  width: 145px;
  height: 145px;
}
.vGauge-base {
  fill: #edebeb;
}
.vGauge-progress {
  transition: 0.5s;
}
.vGauge-val {
  font-weight: 700;
  font-size: 1.3em;
}
.vGauge-min-val, .vGauge-max-val {
  fill: #b3b3b3;
} 
  </style>
  <style>
body {
  margin: 0;
  padding: 20px;
  font-family: Helvetica,Arial,sans-serif;
  font-size: 14px;
  line-height: 1.42857143;
  color: #333;
  background-color: #fff;
}
.gauge-section {
  display: inline-block;
  text-align: center;
}
.gauge-section h3 {
  margin-bottom: 0;
}
.gauge {
  display: inline-block;
}
.actions {
  margin-top: 15px;
}
.btn {
  color: #333;
  border: 0;
  border-radius: 3px;
  box-shadow: 0px 1px 3px #666666;
  background: #fff;
  padding: 10px 20px 10px 20px;
  text-decoration: none;
  cursor: pointer;
}
.btn-icon {
  padding: 2px 15px 2px 15px;
}
.btn:active {
  background: #dddddd;
  text-decoration: none;
} 
  </style>
</head>

<body>
<h1>Electro-osmosis</h1>
<p>
by Radim Keseg
<p/>

<form method='post' action='offset'>
<label>UTC TimeOffset<br><input id='_timeoffset' name='_timeoffset' length=5 pattern='^[0-9-\\.]{1,5}$' required value='{timeoffset}'></label><br>
<label>DST<br><input id='_dst' name='_dst' type='checkbox' {dst}></label><br>
<input type='submit' value='Store'></form>


  <div>
    <div class="gauge-section">
      <h3>Moisture</h3>
      <div id="mois" class="gauge"></div>
    </div>
    <div class="gauge-section">
      <h3>Temperature</h3>
      <div id="temp" class="gauge"></div>
    </div>
  </div>
  <div class="actions">
    <button id="refresh" class="btn btn-icon">
      <svg height="30" viewBox="0 0 24 24" width="30">
          <path d="M17.65 6.35C16.2 4.9 14.21 4 12 4c-4.42 0-7.99 3.58-7.99 8s3.57 8 7.99 8c3.73 0 6.84-2.55 7.73-6h-2.08c-.82 2.33-3.04 4-5.65 4-3.31 0-6-2.69-6-6s2.69-6 6-6c1.66 0 3.14.69 4.22 1.78L13 11h7V4l-2.35 2.35z"/>
          <path d="M0 0h24v24H0z" fill="none"/>
      </svg>
    </button>
  </div>

  <script>
function createRadGauge(id, minVal, maxVal, unit) {
  function polarToCartesian(centerX, centerY, radius, rad) {
    return {
      x: centerX + (radius * Math.cos(rad)),
      y: centerY + (radius * Math.sin(rad))
    };
  }
  
  function arc(x, y, radius, val, minVal, maxVal){
      var start = polarToCartesian(x, y, radius, -Math.PI);
      var end = polarToCartesian(x, y, radius, -Math.PI*(1 - 1/(maxVal-minVal) * (val-minVal)));
  
      var d = [
          "M", start.x, start.y, 
          "A", radius, radius, 0, 0, 1, end.x, end.y
      ].join(" ");
  
      return d;       
  }

  var tmpl = 
  '<svg class="rGauge" viewBox="0 0 200 145">'+ 
    '<path class="rGauge-base" id="'+id+'_base" stroke-width="30" />'+ 
    '<path class="rGauge-progress" id="'+id+'_progress" stroke-width="30" stroke="#1565c0" />'+ 
    '<text class="rGauge-val" id="'+id+'_val" x="100" y="105" text-anchor="middle"></text>'+  
    '<text class="rGauge-min-val" id="'+id+'_minVal" x="40" y="125" text-anchor="middle"></text>'+  
    '<text class="rGauge-max-val" id="'+id+'_maxVal" x="160" y="125" text-anchor="middle"></text>'+  
  '</svg>';

  document.getElementById(id).innerHTML = tmpl;
  document.getElementById(id+'_base').setAttribute("d", arc(100, 100, 60, 1, 0, 1));
  document.getElementById(id+'_progress').setAttribute("d", arc(100, 100, 60, minVal, minVal, maxVal));
  document.getElementById(id+'_minVal').textContent = minVal;
  document.getElementById(id+'_maxVal').textContent = maxVal;

  var gauge = {
    setVal: function(val) {
      val = Math.max(minVal, Math.min(val, maxVal));
      document.getElementById(id+'_progress').setAttribute("d", arc(100, 100, 60, val, minVal, maxVal));
      document.getElementById(id+'_val').textContent = val + (unit !== undefined ? unit: '');
      return gauge;
    },
    setColor: function(color) {
       document.getElementById(id+'_progress').setAttribute("stroke", color);
       return gauge;
    }
  }
  
  return gauge;
}

function createVerGauge(id, minVal, maxVal, unit) {
  var tmpl = 
  '<svg class="vGauge" viewBox="0 0 145 145">'+
    '<rect class="vGauge-base" id="'+id+'_base" x="30" y="25" width="30" height="100"></rect>'+
    '<rect class="vGauge-progress" id="'+id+'_progress" x="30" y="25" width="30" height="0" fill="#1565c0"></rect>'+
    '<text class="vGauge-val" id="'+id+'_val" x="70" y="80" text-anchor="start"></text>'+
    '<text class="vGauge-min-val" id="'+id+'_minVal" x="70" y="125"></text>'+
    '<text class="vGauge-max-val" id="'+id+'_maxVal" x="70" y="30" text-anchor="start"></text>'+
  '</svg>';
  
  document.getElementById(id).innerHTML = tmpl;
  document.getElementById(id+'_minVal').textContent = minVal;
  document.getElementById(id+'_maxVal').textContent = maxVal;
  
  var gauge = {
    setVal: function(val) {
      val = Math.max(minVal, Math.min(val, maxVal));
      var height = 100/(maxVal-minVal) * (val-minVal);
      
      document.getElementById(id+'_progress').setAttribute("height", height);
      document.getElementById(id+'_progress').setAttribute("y", 25+(100-height));
      document.getElementById(id+'_val').textContent = val + (unit !== undefined ? unit: '');
      return gauge;
    },
    setColor: function(color) {
       document.getElementById(id+'_progress').setAttribute("fill", color);
       return gauge;
    }
  }
  
  return gauge;
}     
  </script>
  <script>
function getTempColor(t) {
  if (t >= 35) {
    return '#ff5722';
  } else if (t >= 30) {
    return '#ff9800';
  } else if (t >= 25) {
    return '#ffc107';
  } else if (t >= 18) {
    return '#4caf50';
  } else if (t > 10) {
    return '#8bc34a';
  } else if (t >= 5) {
    return '#00bcd4';
  } else if (t >= -5) {
    return '#03a9f4';
  } else {
    return '#2196f3';
  }
}

function getMoisColor(x) {
  var colors = ['#E3F2FD','#BBDEFB','#90CAF9','#64B5F6','#42A5F5','#2196F3','#1E88E5','#1976D2','#1565C0','#0D47A1','#0D47A1'];
  return colors[Math.round(x/10)];
}

function refresh() {
  var xmlHttp = new XMLHttpRequest();

  xmlHttp.onreadystatechange = function()
  {
    if (xmlHttp.readyState == XMLHttpRequest.DONE) {
        if (xmlHttp.status == 200)
        {
          var data = JSON.parse(xmlHttp.responseText);

          tempGauge.setVal(data.temp).setColor(getTempColor(data.temp));
          moisGauge.setVal(data.mois).setColor(getMoisColor(data.mois));
        } else {
          console.log('Refresh failed: ' + xmlHttp.status);
        }
    }
  }

  xmlHttp.open("GET", "data", true);
  xmlHttp.send();
}

var tempGauge = createVerGauge('temp', -20, 60, ' °C').setVal(0).setColor(getTempColor(0));
var moisGauge = createRadGauge('mois', 0, 100, '%').setVal(0).setColor(getMoisColor(0));

document.getElementById('refresh').addEventListener('click', refresh);
setTimeout(refresh, 100);   
  </script>


</body>
</html>
)=====";
