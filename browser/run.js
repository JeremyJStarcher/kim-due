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

	const run = () => {
		setTimeout(() => {
			try {
				_websetup();

				wireupKeyboard();
				runloop();
			} catch (err) {
				run();
			}
		}, 250)
	};
	run();

}, false);

function setLed(pos, data) {
	const el = document.querySelector(`#display-${pos}`);

	if (!el) {
		return;
	}

	const s = "gfedcba".split("").map((a, i) => {
		const seg = el.getElementById(a);
		seg.style.fill = (data & 1 << i) ? "red" : "#3f0020";
	});
}

function runloop() {
	_webloop();
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
