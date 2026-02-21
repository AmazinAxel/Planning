#pragma once
static const char CSS[] = R"CSS(
	:root {
		/* Polar Night colors */
		--darkest1: #2e3440;
		--darkest2: #3b4252;
		--darkest3: #434c5e;
		--darkest4: #4c566a;

		/* Snow Storm colors */
		--lightest1: #eceff4;
		--lightest2: #e5e9f0;
		--lightest3: #d8dee9;

		/* Nord Aurora misc colors */
		--red: #bf616a;
		--orange: #d08770;
		--yellow: #ebcb8b;
		--green: #a3be8c;
		--purple: #b48ead;

		/* Frost */
		--blue1: #b5d2d9; /* Permafrost Blue */
		--blue2: #81a1c1;
		--blue3: #5e81ac;
	}

	/* App */

	window { padding: 1rem; }

	.appHeader { padding-bottom: 1rem; }

	label.headerText {
		font-size: 2rem;
		font-family: 'Hammersmith One';
		color: var(--lightest3);
	}

	/* Plan list */

	button.planName {
		font-size: 1.5rem;
		font-family: 'Sora';
	}

	label.isSynced { color: var(--darkest4); }

	/* No annoying button outlines */
	button { outline-width: 0px; outline: none; }
)CSS";
