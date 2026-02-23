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

	.appHeader button { padding: 0 0.75rem; } /* Make buttons actually circular */

	.appHeader { padding-bottom: 1rem; }

	label.headerText {
		font-size: 2.5rem;
		font-family: 'Hammersmith One';
		color: var(--lightest3);
	}

	.planPage { background-color: var(--darkest1); }

	/* Plan list */
	.planListHeader { padding: 8rem 0 1.5rem 0; }

	button.planName {
		font-size: 2rem;
		font-family: 'Sora';
		color: var(--lightest3);
		padding: 1rem 0;
		min-width: 25rem;
	}

	label.isSynced { color: var(--darkest4); }

	entry.indented text { padding-left: 10px; }

	.addNewPlanButton {
		-gtk-icon-size: 1.75rem;
		padding: 0.5rem;
		min-width: 4rem;
		min-height: 4rem;
	}

	/* Lists */
	.listHeader {
		font-family: 'Sora';
		font-size: 1.4rem;
		font-weight: bold;
		color: var(--lightest2);
		padding: 0.2rem;
	}
	.listSection * { border-radius: 0; }
	.listSection { border-radius: 0.5rem; }

	/* No annoying button outlines */
	button { outline-width: 0px; outline: none; }
)CSS";
