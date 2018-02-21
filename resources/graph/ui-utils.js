		//MIT License
		//
		//Copyright (c) 2018 Luís Victor Müller Fabris
		//
		//Permission is hereby granted, free of charge, to any person obtaining a copy
		//of this software and associated documentation files (the "Software"), to deal
		//in the Software without restriction, including without limitation the rights
		//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
		//copies of the Software, and to permit persons to whom the Software is
		//furnished to do so, subject to the following conditions:
		//
		//The above copyright notice and this permission notice shall be included in all
		//copies or substantial portions of the Software.
		//
		//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
		//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
		//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
		//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
		//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
		//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
		//SOFTWARE.

		//From https://stackoverflow.com/questions/5560248/programmatically-lighten-or-darken-a-hex-color-or-rgb-and-blend-colors
		var cntdihidhiggfidgf = 0;
		function isLetter(stryunt) {
			if (stryunt.replace(/[^:'A-Za-z0-9]+/g, "") + "" == "" + stryunt) {
				return 1;
			} else {
				return 0;
			}
		}
		function LightenDarkenColor(col, amt) {
			var usePound = false;
			if (col[0] == "#") {
				col = col.slice(1);
				usePound = true;
			}

			var num = parseInt(col, 16);

			var r = (num >> 16) + amt;

			if (r > 255) r = 255;
			else if (r < 0) r = 0;

			var b = ((num >> 8) & 0x00FF) + amt;

			if (b > 255) b = 255;
			else if (b < 0) b = 0;

			var g = (num & 0x0000FF) + amt;

			if (g > 255) g = 255;
			else if (g < 0) g = 0;

			return (usePound ? "#" : "") + (g | (b << 8) | (r << 16)).toString(16);
		}
		//From https://stackoverflow.com/questions/5623838/rgb-to-hex-and-hex-to-rgb
		function hexToRgb(hex) {
			// Expand shorthand form (e.g. "03F") to full form (e.g. "0033FF")
			var shorthandRegex = /^#?([a-f\d])([a-f\d])([a-f\d])$/i;
			hex = hex.replace(shorthandRegex, function (m, r, g, b) {
				return r + r + g + g + b + b;
			});

			var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
			return result ? {
				r: parseInt(result[1], 16),
				g: parseInt(result[2], 16),
				b: parseInt(result[3], 16)
			} : null;
		}
		//From https://stackoverflow.com/questions/5623838/rgb-to-hex-and-hex-to-rgb
		function componentToHex(c) {
			var hex = c.toString(16);
			return hex.length == 1 ? "0" + hex : hex;
		}
		//From https://stackoverflow.com/questions/5623838/rgb-to-hex-and-hex-to-rgb
		function rgbToHex(r, g, b) {
			return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
		}
		//From https://stackoverflow.com/questions/35969656/how-can-i-generate-the-opposite-color-according-to-current-color (modified)
		function invertColor(hex) {
			if (hex.indexOf('#') === 0) {
				hex = hex.slice(1);
			}
			// convert 3-digit hex to 6-digits.
			if (hex.length === 3) {
				hex = hex[0] + hex[0] + hex[1] + hex[1] + hex[2] + hex[2];
			}
			if (hex.length !== 6) {
				console.log(hex);
				throw new Error('Invalid HEX color.');

			}
			// invert color components
			var r = ( 255- parseInt(hex.slice(2, 4), 16)).toString(16),
				g = ( 100- parseInt(hex.slice(0, 2), 16)).toString(16),
				b = (255 - parseInt(hex.slice(4, 6), 16)).toString(16);

			var rgjsghfg = hexToRgb('#' + padZero(g) + padZero(b) + padZero(r)).r;
			var ggjsghfg = hexToRgb('#' + padZero(g) + padZero(b) + padZero(r)).g;
			var bgjsghfg = hexToRgb('#' + padZero(g) + padZero(b) + padZero(r)).b;
			if (rgjsghfg > 160) {
				rgjsghfg = rgjsghfg - 150;
			} else {
				if (rgjsghfg < 30) {
				    rgjsghfg = rgjsghfg + 210;
				}
			}

			if (ggjsghfg < 50) {
				ggjsghfg = ggjsghfg + 90;
			}
			// pad each with zeros and return
			return detectacontraste(rgbToHex(rgjsghfg, ggjsghfg, bgjsghfg),hex);
		}
		//From https://stackoverflow.com/questions/35969656/how-can-i-generate-the-opposite-color-according-to-current-color
		function padZero(str, len) {
			len = len || 2;
			var zeros = new Array(len).join('0');
			return (zeros + str).slice(-len);
		}
		function detectacontraste(corcontraste, corcontrastefundo) {
			var rcontraste = Number(hexToRgb(corcontraste).r);
			var gcontraste = Number(hexToRgb(corcontraste).g);
			var bcontraste = Number(hexToRgb(corcontraste).b);
			var rcontrastefundo = Number(hexToRgb(corcontrastefundo).r);
			var gcontrastefundo = Number(hexToRgb(corcontrastefundo).g);
			var bcontrastefundo = Number(hexToRgb(corcontrastefundo).b);

			if (Math.abs((0.2126 * rcontraste + 0.7152 * gcontraste + 0.0722 * bcontraste) - (0.2126 * rcontrastefundo + 0.7152 * gcontrastefundo + 0.0722 * bcontrastefundo)) > 220) {
				return LightenDarkenColor(corcontrastefundo, -20);
			} else {
				return corcontraste;
			}

		}
		var dadosanterioresatualizar=[]
		function updatespectrumfrompython(){
				if(spectrumdata+""!="undefined"){
					dados=spectrumdata.split(" ");
					if(dadosanterioresatualizar+""!=dados+""){
						atualizadados(dados,120,1000,1500);
						dadosanterioresatualizar=dados;
					}
			}
		}
		function sendtopython() {
				document.title = "null";
				document.title = ""+Math.random();
			}
		setInterval("sendtopython()",80);
		setInterval("updatespectrumfrompython()",110);
		function convertecorvbvbv(hexcodejggg) {
			if (hexcodejggg.indexOf("#") + "" == "-1") {
				hexcodejggg = "#" + hexcodejggg;
			}
			if (hexcodejggg.length + "" == "13") {
				var parte1hggh = hexcodejggg.substr(1, 2);
				var parte2hggh = hexcodejggg.substr(5, 2);
				var parte3hggh = hexcodejggg.substr(9, 2);
				return "#" + parte1hggh + "" + parte2hggh + "" + parte3hggh + "";
			} else {
				return hexcodejggg;
			}
		}
		var oldsyscolors="";
		function updategtktheme(){
			var intupdategtktheme="";
			try{
			if(""+oldsyscolors!=""+systemcolors){
				if(systemcolors+""!=""){
					if(systemcolors.indexOf("text-normal-color:")+""!="-1"){
						//corlinhagrafico = invertColor(convertecorvbvbv("" + ((systemcolors.split("text-input-normal:")[1]).split(";")[0]) + ""));
						if(systemcolors.split("text-input-selected-unsafe:")[1].split(";")[0]+""!="#000000"){
						corlinhagrafico = (convertecorvbvbv("" + ((systemcolors.split("text-input-selected-unsafe:")[1]).split(";")[0]) + ""));
						}else{
						corlinhagrafico = invertColor(convertecorvbvbv("" + ((systemcolors.split("text-input-normal:")[1]).split(";")[0]) + ""));
						}
						changeglobalgraphtextcolor(""+convertecorvbvbv(""+systemcolors.split("text-normal-color:")[1].split(";")[0]));
						atualizadadosb();
						//console.log(systemcolors.split("menu-item-active-unsafe:")[1].split(";")[0]);
						oldsyscolors=systemcolors;
						}
					}
				}
			}catch(e){}
		}
		setInterval("updategtktheme()",30);
		setTimeout("updategtktheme()",3);
		setTimeout("updategtktheme()",15);
		function drawtoui(){
			if((performance.now()-lastredraw)>150){
				lastredraw=performance.now();
				document.getElementById("detected_peaks").innerHTML="Peaks:"+picos;
			}
		}
