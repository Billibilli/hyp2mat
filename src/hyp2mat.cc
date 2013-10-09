/*
 * hyp2mat - convert hyperlynx files to matlab scripts
 * Copyright 2012 Koen De Vleeschauwer.
 *
 * This file is part of hyp2mat.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <unistd.h>
#include <iostream>

#include "hyp2mat.h"
#include "cmdline.h"

using namespace Hyp2Mat;

int main(int argc, char **argv)
{
  try {
    PCB pcb;
  
    /*
     * parse command line 
     */

    gengetopt_args_info args_info;
  
    if (cmdline_parser (argc, argv, &args_info) != 0)
      exit(1);
  
    /* set layers to import */
    std::vector<std::string> layers;
    if (args_info.layer_given && args_info.layer_arg)
      for (unsigned int i = 0; i < args_info.layer_given; ++i)
        layers.push_back(args_info.layer_arg[i]);
      
    /* set nets to import */
    std::vector<std::string> nets;
    if (args_info.net_given && args_info.net_arg)
      for (unsigned int i = 0; i < args_info.net_given; ++i)
        nets.push_back(args_info.net_arg[i]);
      
    /* set input filename */
    std::string input_file = "-";

    if (args_info.inputs_num == 1)
      input_file = args_info.inputs[0];

    if (args_info.inputs_num > 1) {
      std::cerr << "more than one input file given" << std::endl;
      exit(1);
      }
      
    /* raw processing */
    bool raw_flag = args_info.raw_flag; 

    /* output file */
    std::string output_file = args_info.output_arg;

    /*
     * Process hyperlynx file
     */

    /* set debug level */
    pcb.debug = args_info.debug_given;

    /* optionally set grid */
    if (args_info.grid_given)
      pcb.SetGrid(args_info.grid_arg);
 
    /* optionally set arc precision */
    if (args_info.arc_precision_given)
      pcb.SetArcPrecision(args_info.arc_precision_arg);
 
    /* optionally set trace-to-plane clearance */
    if (args_info.clearance_given)
      pcb.SetClearance(args_info.clearance_arg);

    /* set plane layer flooding */
    pcb.flood_plane_layers =  args_info.flood_flag;
 
    /* optionally crop */
    Bounds bounds;
    if (args_info.xmin_given) bounds.x_min = args_info.xmin_arg;
    if (args_info.xmax_given) bounds.x_max = args_info.xmax_arg;
    if (args_info.ymin_given) bounds.y_min = args_info.ymin_arg;
    if (args_info.ymax_given) bounds.y_max = args_info.ymax_arg;
    if (args_info.zmin_given) bounds.z_min = args_info.zmin_arg;
    if (args_info.zmax_given) bounds.z_max = args_info.zmax_arg;
    if (args_info.xmin_given || args_info.xmax_given || 
        args_info.ymin_given || args_info.ymax_given ||
        args_info.zmin_given || args_info.zmax_given )
      pcb.SetBounds(bounds);

    /* optionally set epsilon */
    if (args_info.epsilonr_given)
      pcb.SetEpsilonR(args_info.epsilonr_arg);
 
    /* load hyperlynx file */
    pcb.ReadHyperLynx(input_file, layers, nets, raw_flag);
  
    /* optionally print layer summary */
    if (args_info.verbose_given)
      pcb.PrintSummary();
 
    /*
     * write output 
     */
  
    switch(args_info.output_format_arg) {
      case output_format_arg_pdf:
        if (!args_info.output_given) 
          std::cerr << "pdf output file missing " << std::endl;
        else  
          pcb.WritePDF(output_file, args_info.hue_arg, args_info.saturation_arg, args_info.brightness_arg);
        break;
      case output_format_arg_csxcad:
        pcb.WriteCSXCAD(output_file);
        break;
      default:
        std::cerr << "unknown output format " << args_info.output_format_arg << std::endl;
        break;
      }
    
    /* exit */
    exit(0);

  }
  catch(...) {
    exit(1);
    }

}
/* not truncated */
