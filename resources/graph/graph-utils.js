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

 		var corlinhagrafico = "rgba(0,0,0,0)";
        desenhagrafico();
        var globalgraphtextcolor = "rgba(0,0,0,0)";
        function changeglobalgraphtextcolor(corhdfhjghf) {
            globalgraphtextcolor = corhdfhjghf;
        }
        var chart = nv.models.lineChart();
        function desenhagrafico() {
            document.getElementById("chart1").innerHTML = "<svg></svg>";
            nv.addGraph(function () {

                var fitScreen = true;
                var width = 600;
                var height = 300;
                var zoom = 1.2;

                chart.useInteractiveGuideline(false);
                chart.xAxis.tickFormat(d3.format(',r'));
				chart.tooltip.enabled(false);
				chart.duration(0);
				chart.lines.interactive(false);
                function desenhagfef() {
                    chart.yAxis
                        .axisLabel('Intensity')
                        .tickFormat(d3.format(',.4s'));

                    chart.xAxis
                        .axisLabel('Wavelength (nm)')
                        .tickFormat(d3.format(',.2f'));

                    d3.select('#chart1 svg')
                        .attr('perserveAspectRatio', 'xMinYMid')
                        .attr('width', width)
                        .attr('height', height)
                        .datum(sinAndCos());

                    setChartViewBox();
                    resizeChart();
                    nv.utils.windowResize(resizeChart);
                }
                desenhagfef();
                function setChartViewBox() {
                    var w = width * zoom,
                        h = height * zoom;

                    chart
                        .width(w)
                        .height(h);

                    d3.select('#chart1 svg')
                        .attr('viewBox', '0 0 ' + w + ' ' + h)
                        .attr("fill", "" + globalgraphtextcolor + "")

                        .call(chart);
                }
                function zoomOut() {
                    zoom += .25;
                    setChartViewBox();
                }
                function zoomIn() {
                    if (zoom <= .5) return;
                    zoom -= .25;
                    setChartViewBox();
                }
                // This resize simply sets the SVG's dimensions, without a need to recall the chart code
                // Resizing because of the viewbox and perserveAspectRatio settings
                // This scales the interior of the chart unlike the above
                function resizeChart() {
                    var container = d3.select('#chart1');
                    var svg = container.select('svg');

                    if (fitScreen) {
                        // resize based on container's width AND HEIGHT
                        var windowSize = nv.utils.windowSize();
                        svg.attr("width", windowSize.width);
                        svg.attr("height", windowSize.height);
                    } else {
                        // resize based on container's width
                        var aspect = chart.width() / chart.height();
                        var targetWidth = parseInt(container.style('width'));
                        svg.attr("width", targetWidth);
                        svg.attr("height", Math.round(targetWidth / aspect));
                    }
                }
                return chart;
            });
            function sinAndCos() {
                var sin = [],
                    cos = [];
                var fftyftyt = 90;
                for (var i = 1; i <= fftyftyt; i++) {
                    sin.push({ x: (i), y: 0 });
                }
                return [
                    {
                        values: sin,
                        key: "",
                        color: "" + corlinhagrafico
                    },
                ];
            }
        }
		function atualizadadosc(dadosatualizadadosb,fftyftytatualizadadosb,inicioescalaatualizadadosb,fimescalaatualizadadosb) {
                var sin = [],
                    cos = [];
                for (var i = 0; i <= fftyftytatualizadadosb; i++) {
				//console.log(" x:"+ inicioescalaatualizadadosb+", y:"+ dadosatualizadadosb[i])
                    sin.push({ x: (((fimescalaatualizadadosb-inicioescalaatualizadadosb)*i/fftyftytatualizadadosb)+inicioescalaatualizadadosb), y: Number(dadosatualizadadosb[i])});
                }
                return [
                    {
                        values: sin,
                        key: "",
                        color: "" + corlinhagrafico
                    },
                ];
         }
		function forceupdate() {
                var sin = [],
                    cos = [];
                var fftyftyt = 90;
                for (var i = 0; i <= fftyftyt; i++) {
                    sin.push({ x: (i), y: 0});
                }
                return [
                    {
                        values: sin,
                        key: "",
                        color: "" + corlinhagrafico
                    },
                ];
         }
		//Atualizar grafico (vetor dados, npontos, inicioescala,fimescala)
		function atualizadados(dadosatualizadados,fftyftytatualizadados,inicioescalaatualizadados,fimescalaatualizadados) {
			//console.log(dadosatualizadados.length);
			d3.select('#chart1 svg').datum(atualizadadosc(dadosatualizadados,fftyftytatualizadados,inicioescalaatualizadados,fimescalaatualizadados)).call(chart);
		}
		function atualizadadosb() {
			d3.select('#chart1 svg').datum(forceupdate()).attr("fill", "" + globalgraphtextcolor + "").call(chart);
		}
		desenhagrafico();
