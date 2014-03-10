var fromX, fromY;

var animationNames = [];
var counter = {};
var i = 0;

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

var registerAnimations = function(input)
{
	input = input.rawString();
	var key = input.substring(0,input.indexOf("__"));
	if (animationNames.indexOf(key) === -1)
	{
		counter[key] = 0;
		animationNames.push(key);
	}
	else
		counter[key] += 1;
}

var animationLoopBegin = function(input)
{
	i = 0;
	if (animationNames.length > 0)
	{
		return "true";
	}
	return "";
}

var animationLoopEnd = function(input)
{
	animationNames.pop();
	if (animationNames.length <= 0)
		return "true";
	return "";
}

var getAnimationName = function(input)
{
	return animationNames[animationNames.length - 1];
}

var isInAnimation = function(input)
{
	input = input.rawString();
	var key = input.substring(0,input.indexOf("__"));
	if (animationNames.indexOf(key) === animationNames.length - 1)
	{
		return "true";
	}
	return "";
}

var isLastFrame = function(input)
{
	i++;
	input = input.rawString();
	var key = input.substring(0,input.indexOf("__"));
	if (counter[key] < i)
		return "true"
	return "";
}

var margin = [0,0];

var marginX = function(input)
{
	margin[0] = +input;
	return input;
}

var marginY = function(input)
{
	margin[1] = +input;
	return input;
}

var posToX = function(input)
{
	return ""+(margin[0] + (+input));
}

var posToY = function(input)
{
	return ""+(margin[1] + (+input));
}

// the filter name

uvFromX.filterName = "uvfromx";
uvFromY.filterName = "uvfromy";
uvToX.filterName = "uvtox";
uvToY.filterName = "uvtoy";
registerAnimations.filterName = "registerAnimations";
animationLoopBegin.filterName = "animationLoopBegin";
animationLoopEnd.filterName = "animationLoopEnd";
getAnimationName.filterName = "getAnimationName";
isInAnimation.filterName = "isInAnimation";
isLastFrame.filterName = "isLastFrame";
marginX.filterName = "marginX";
marginY.filterName = "marginY";
posToX.filterName = "posToX";
posToY.filterName = "posToY";
 

// register the filter

Library.addFilter("uvFromX");
Library.addFilter("uvFromY");
Library.addFilter("uvToX");
Library.addFilter("uvToY");
Library.addFilter("registerAnimations");
Library.addFilter("animationLoopBegin");
Library.addFilter("animationLoopEnd");
Library.addFilter("getAnimationName");
Library.addFilter("isInAnimation");
Library.addFilter("isLastFrame");
Library.addFilter("marginX");
Library.addFilter("marginY");
Library.addFilter("posToX");
Library.addFilter("posToY");