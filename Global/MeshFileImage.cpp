/*
 * MeshFileImage.cpp
 *
 *  Created on: 26/11/2017
 *      Author: asantos
 */
#include "MeshFile.h"
#include <string.h>
#include <functional>
#include <iostream>
#include <libps/pslib.h>

#include "Node.h"

int colorline(PSDoc *ps, float leftborder, struct spotcolor *spot) {
	int i;
	int psfont;
	char buffer[100];
	for(i=1; i<=10; i++) {
		PS_setcolor(ps, "fill", "spot", (float) spot->id, i*0.1, 0.0, 0.0);
		PS_rect(ps, leftborder, 35+i*65, 50, 50);
		PS_fill(ps);
	}
	PS_setcolor(ps, "stroke", "gray", 0.0, 0.0, 0.0, 0.0);
	psfont = PS_findfont(ps, "Helvetica", "", 0);
	PS_setfont(ps, psfont, 7.0);
	PS_show_xy(ps, spot->name, leftborder, 100+10*65+13);
	if(!strcmp(spot->colorspace, "cmyk")) {
		sprintf(buffer, "%.2f, %.2f, %.2f, %.2f", spot->c1, spot->c2, spot->c3, spot->c4);
		PS_show_xy(ps, buffer, leftborder, 100+10*65+3);
	} else if(!strcmp(spot->colorspace, "rgb")) {
		sprintf(buffer, "%.2f, %.2f, %.2f", spot->c1, spot->c2, spot->c3);
		PS_show_xy(ps, buffer, leftborder, 100+10*65+3);
	}
	return(spot->id);
}

void MeshFile::showImage() {
	PSDoc *ps;
	PS_boot();
	ps = PS_new();
	if (0 > PS_open_file(ps, "psclock.ps")) {
		printf("Cannot open PostScript file\n");
		exit(1);
	}
	PS_set_parameter(ps, "warning", "true");
	PS_set_info(ps, "Creator", __FILE__);
	PS_set_info(ps, "Author", "André Santos");
	PS_set_info(ps, "Title", "Imagem TIE");
	PS_set_info(ps, "Keywords", "Reconstrução");

	psfont = PS_findfont(ps, "Helvetica", "", 0);
	PS_setfont(ps, psfont, 7.0);
	for(i=1; i<=10; i++) {
		char buffer[10];
		sprintf(buffer, "Teste");
		PS_show_xy(ps, buffer, 60, 55+i*65);
	}

	/* Find Size */
	unsigned long min_x, max_x, min_y, max_y;
	std::list<unsigned long, Node>::iterator elem;
	elem = nodes.begin();
	min_x = elem->second.x;
	max_x = elem->second.x;
	min_y = elem->second.y;
	max_y = elem->second.y;
	elem++;

	for(;elem != nodes.end(); elem++) {
		if (min_x > elem->second.x)
			min_x = elem->second.x;
		if (max_x < elem->second.x)
			max_x = elem->second.x;
		if (min_y > elem->second.y)
			min_y = elem->second.y;
		if (max_y < elem->second.y)
			max_y = elem->second.y;
	}

	PS_begin_page(ps, max_x - min_x, max_y - min_y);
	PS_set_parameter(ps, "transition", "wipe");
	PS_set_value(ps, "duration", 0.5);

  std::list<GenericElement*>::iterator it;
  std::map<unsigned long, unsigned long>::iterator elem;
  elem = gNodes.find(nodeNumber);
  if (gNodes.end() != elem)
    return elem->second;
  return getNodesSize() + 1;

	PS_translate(ps, radius + margin, radius + margin);

	/* minute strokes */
	PS_save(ps);
	PS_setcolor(ps, "stroke", "rgb", 0.0, 0.0, 1.0, 0.0);
	PS_setlinewidth(ps, 2.0);



	PS_end_page(ps);


	/*buf = PS_get_buffer(ps);
	len = strlen(buf);

	printf("Content-type: application/ps");
	printf("Content-Length: len");
	printf("Content-Disposition: inline; filename=foo.pdf");
	printf("\n\n");
	printf(buf);
	*/
	PS_end_page(ps);
	PS_close(ps);
	PS_delete(ps);
	PS_shutdown();
}



