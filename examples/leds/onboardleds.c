/*
 * onboardleds.h
 *
 *  Created on: Jul 28, 2015
 *      Author: gijs
 */

#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "onboardleds.h"
#include "../../src/log.h"

void saveOriginalLedTrigger(led_info *led) {
//	printf("Get Led trigger %d", led->lednr);
	FILE *fd;
	int i, j = 0, start = -1, end = -1;
	char buf[BUF_SIZE];
	snprintf(buf, sizeof(buf), leddir "trigger", led->lednr);

	fd = fopen(buf, "r");
	if (fd < 0) {
		error("On board leds: could not get led trigger");
	}

	char str[1024];
	fgets(str, 1024, fd);
	for(i = 0; i < 1024; i++) {
		if (str[i] == '[')
			start = i + 1;
		if (str[i] == ']')
			end = i;
	}
	if (start == -1 || end == -1) {
		error("Could not find trigger in values: %s\n", str);
	}
	led->org_trigger = malloc(sizeof(char)*((end - start) + 1));

	for( i = start; i < end; i++) {
		led->org_trigger[j++] = str[i];
	}
	led->org_trigger[end] = '\0';
	fclose(fd);
}

void setLedTrigger(led_info *led, char *value) {
//	printf("Set Led trigger %d", led->lednr);
	FILE *fd;
	char buf[BUF_SIZE];
	snprintf(buf, sizeof(buf), leddir "trigger", led->lednr);

	fd = fopen(buf, "w");
	fputs(value, fd);
	fclose(fd);
}

void saveOriginalLedBrightness(led_info *led) {
	FILE *fd;
	char buf[BUF_SIZE];
	snprintf(buf, sizeof(buf), leddir "brightness", led->lednr);

	fd = fopen(buf, "r");
	char str[2];
	fgets(str, 2, fd);
	led->org_brightness = atoi(str);
	fclose(fd);
}

void setLedBrightness(led_info *led, int value) {
	FILE *fd;
	char buf[BUF_SIZE];
	snprintf(buf, sizeof(buf), leddir "brightness", led->lednr);

	fd = fopen(buf, "w");
	char str[2];
	sprintf(str, "%d", value);
	fputs(str, fd);
	fclose(fd);
}

void saveState() {
	saveOriginalLedTrigger(led0);
	saveOriginalLedBrightness(led0);
	saveOriginalLedTrigger(led1);
	saveOriginalLedBrightness(led1);
	saveOriginalLedTrigger(led2);
	saveOriginalLedBrightness(led2);
	saveOriginalLedTrigger(led3);
	saveOriginalLedBrightness(led3);
}

void resetState() {
	setLedTrigger(led0, led0->org_trigger);
	setLedBrightness(led0, led0->org_brightness);
	setLedTrigger(led1, led1->org_trigger);
	setLedBrightness(led1, led1->org_brightness);
	setLedTrigger(led2, led2->org_trigger);
	setLedBrightness(led2, led2->org_brightness);
	setLedTrigger(led3, led3->org_trigger);
	setLedBrightness(led3, led3->org_brightness);
}

void clearLeds() {
	setLedTrigger(led0, "none");
	setLedBrightness(led0, 0);
	setLedTrigger(led1, "none");
	setLedBrightness(led1, 0);
	setLedTrigger(led2, "none");
	setLedBrightness(led2, 0);
	setLedTrigger(led3, "none");
	setLedBrightness(led3, 0);
}

void walker() {
	int i;
	for(i = 0; i < 5; i++) {
		setLedBrightness(led0, 1);
		usleep(DELAY/3);
		setLedBrightness(led0, 0);
		usleep(DELAY/3);
		setLedBrightness(led1, 1);
		usleep(DELAY/3);
		setLedBrightness(led1, 0);
		usleep(DELAY/3);
		setLedBrightness(led2, 1);
		usleep(DELAY/3);
		setLedBrightness(led2, 0);
		usleep(DELAY/3);
		setLedBrightness(led3, 1);
		usleep(DELAY/3);
		setLedBrightness(led3, 0);
		usleep(DELAY/3);
	}
}

void filler() {
	int i;
	for(i = 0; i < 5; i++) {
		setLedBrightness(led0, 1);
		usleep(DELAY);
		setLedBrightness(led1, 1);
		usleep(DELAY);
		setLedBrightness(led2, 1);
		usleep(DELAY);
		setLedBrightness(led3, 1);
		usleep(DELAY);
		clearLeds();
		usleep(DELAY);
	}
}

void shader() {
	int i;
	for(i = 0; i < 5; i++) {
		setLedBrightness(led0, 1);
		setLedBrightness(led1, 1);
		setLedBrightness(led2, 1);
		setLedBrightness(led3, 1);
		usleep(DELAY);

		setLedBrightness(led0, 0);
		usleep(DELAY);
		setLedBrightness(led0, 1);
		setLedBrightness(led1, 0);
		usleep(DELAY);
		setLedBrightness(led1, 1);
		setLedBrightness(led2, 0);
		usleep(DELAY);
		setLedBrightness(led2, 1);
		setLedBrightness(led3, 0);
		usleep(DELAY);
	}
}

void flipflop() {
	int i;
	for(i = 0; i < 5; i++) {
		setLedBrightness(led0, 1);
		setLedBrightness(led1, 0);
		setLedBrightness(led2, 1);
		setLedBrightness(led3, 0);
		usleep(DELAY);
		setLedBrightness(led0, 0);
		setLedBrightness(led1, 1);
		setLedBrightness(led2, 0);
		setLedBrightness(led3, 1);
		usleep(DELAY);
	}
}

int onBoardLedsExample() {
	printf("Running onboard leds example.");
    led0 = malloc(sizeof(led_info));
	led1 = malloc(sizeof(led_info));
	led2 = malloc(sizeof(led_info));
	led3 = malloc(sizeof(led_info));
	led0->lednr = 0;
	led1->lednr = 1;
	led2->lednr = 2;
	led3->lednr = 3;

	saveState();
	clearLeds();
	walker();
	filler();
	shader();
	flipflop();
	resetState();

	free(led0);
	free(led1);
	free(led2);
	free(led3);
	debug("Finished on board leds example");
	return 0;
}
