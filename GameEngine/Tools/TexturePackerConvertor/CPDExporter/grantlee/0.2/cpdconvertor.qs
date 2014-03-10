var fromX, fromY;

var uvFromX = function(input)
{
	return input;
}

var uvFromY = function(input)
{
	return input;
}

var uvToX = function(input)
{
	return "0";
}

var uvToY = function(input)
{
	return "0";
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