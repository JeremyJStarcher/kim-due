"use strict";

document.addEventListener('DOMContentLoaded', () => {
	// In order to twiddle the SVG, it must be directly inlined.
	// 
	// For some reason, you can't twiddle SVGs that are loaded as an object
	// or an image.
	// So we inline our prototype and we just make the copies of it as
	// we need

	for (let i = 0; i < 6; i++) {
		const c = document.querySelector("#seg7").cloneNode(true);
		c.id = `display-${i}`;
		const ledbar = document.querySelector(`#ledbar-${i}`);
		ledbar.appendChild(c);
	}
}, false);

const ledTimer = [];

function setLed(pos, data, force) {

	const el = document.querySelector(`#display-${pos}`);

	if (!el) {
		return;
	}

	if (force) {
		ledTimer[pos] = null;
	} else {
		if (data === 0) {
			ledTimer[pos] = cpuInstructionCount;
			return;
		} else {
			ledTimer[pos] = null;
		}
	}

	const s = "gfedcba".split("").map((a, i) => {
		const seg = el.getElementById(a);
		seg.style.fill = (data & 1 << i) ? "red" : "#3f0020";
	});
}

let cpuInstructionCount = 0;
function runloop() {
	const ANTIFLICKER_DELAY = 2000;

	const NUMBER_OF_INSTRUCTIONS = 1000;
	_webloop(NUMBER_OF_INSTRUCTIONS);
	cpuInstructionCount += NUMBER_OF_INSTRUCTIONS;

	for (let i = 0; i < ledTimer.length; i++) {
		let oldCount = ledTimer[i];
		if (oldCount !== null) {
			const instrunctionsSince = cpuInstructionCount - oldCount;
			if (instrunctionsSince > ANTIFLICKER_DELAY) {
				setLed(i, 0, true);
			}
		}
	}

	setTimeout(runloop, 1);
}

function wireupKeyboard() {
	const buttons = document.querySelectorAll("#button-box button");

	Array.from(buttons).forEach((b) => {
		b.addEventListener("click", (e) => {
			e.preventDefault();

			const ch = e.currentTarget.getAttribute("data-asc");
			const v = ch.charCodeAt(0) & 0x7F;
			_injectkey(ch);
		});
	});
}

Module['onRuntimeInitialized'] = onRuntimeInitialized;

function onRuntimeInitialized() {
	_websetup();
	wireupKeyboard();
	runloop();
}
