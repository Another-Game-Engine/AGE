var fromX, fromY;

var uvFromX = function(input)
{
	fromX = +input;
	return input;
}

var uvFromY = function(input)
{
	fromY = +input;
	return input;
}

var uvToX = function(input)
{
	var v = fromX + (+input);
	return ""+v;
}

var uvToY = function(input)
{
	var v = fromY + (+input);
	return ""+v;
}

// the filter name

uvFromX.filterName = "uvfromx";

uvFromY.filterName = "uvfromy";
uvToX.filterName = "uvtox";
uvToY.filterName = "uvtoy";

 

// register the filter

Library.addFilter("uvFromX");
Library.addFilter("uvFromY");
Library.addFilter("uvToX");
Library.addFilter("uvToY");