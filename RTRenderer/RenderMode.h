#pragma once

enum RenderMode {
	// Pre-defined mode that doesn't require user input
	CINEMATIC,
	// User is allowed to control the camera
	ROAM,
	// Bakes static light maps
	BAKING,
	// Undefined mode - doesn't run
	UNDEFINED
};