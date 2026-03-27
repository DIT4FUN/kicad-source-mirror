/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright The KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <api/api_enums.h>
#include <import_export.h>
#include <api/common/types/enums.pb.h>
#include <api/board/board.pb.h>
#include <api/board/board_types.pb.h>
#include <api/schematic/schematic_jobs.pb.h>
#include <api/schematic/schematic_types.pb.h>

#include <core/typeinfo.h>
#include <font/text_attributes.h>
#include <jobs/job_export_sch_netlist.h>
#include <jobs/job_export_sch_plot.h>
#include <layer_ids.h>
#include <drc/drc_item.h>
#include <pin_type.h>
#include <stroke_params.h>
#include <widgets/report_severity.h>

using namespace kiapi;
using namespace kiapi::common;

template<>
KICAD_T FromProtoEnum( types::KiCadObjectType aValue )
{
    switch( aValue )
    {
    case types::KiCadObjectType::KOT_PCB_FOOTPRINT:         return PCB_FOOTPRINT_T;
    case types::KiCadObjectType::KOT_PCB_PAD:               return PCB_PAD_T;
    case types::KiCadObjectType::KOT_PCB_SHAPE:             return PCB_SHAPE_T;
    case types::KiCadObjectType::KOT_PCB_BARCODE:           return PCB_BARCODE_T;
    case types::KiCadObjectType::KOT_PCB_REFERENCE_IMAGE:   return PCB_REFERENCE_IMAGE_T;
    case types::KiCadObjectType::KOT_PCB_FIELD:             return PCB_FIELD_T;
    case types::KiCadObjectType::KOT_PCB_GENERATOR:         return PCB_GENERATOR_T;
    case types::KiCadObjectType::KOT_PCB_TEXT:              return PCB_TEXT_T;
    case types::KiCadObjectType::KOT_PCB_TEXTBOX:           return PCB_TEXTBOX_T;
    case types::KiCadObjectType::KOT_PCB_TABLE:             return PCB_TABLE_T;
    case types::KiCadObjectType::KOT_PCB_TABLECELL:         return PCB_TABLECELL_T;
    case types::KiCadObjectType::KOT_PCB_TRACE:             return PCB_TRACE_T;
    case types::KiCadObjectType::KOT_PCB_VIA:               return PCB_VIA_T;
    case types::KiCadObjectType::KOT_PCB_ARC:               return PCB_ARC_T;
    case types::KiCadObjectType::KOT_PCB_MARKER:            return PCB_MARKER_T;
    case types::KiCadObjectType::KOT_PCB_DIMENSION:         return PCB_DIMENSION_T;
    case types::KiCadObjectType::KOT_PCB_ZONE:              return PCB_ZONE_T;
    case types::KiCadObjectType::KOT_PCB_GROUP:             return PCB_GROUP_T;
    case types::KiCadObjectType::KOT_SCH_GROUP:             return SCH_GROUP_T;
    case types::KiCadObjectType::KOT_SCH_MARKER:            return SCH_MARKER_T;
    case types::KiCadObjectType::KOT_SCH_JUNCTION:          return SCH_JUNCTION_T;
    case types::KiCadObjectType::KOT_SCH_NO_CONNECT:        return SCH_NO_CONNECT_T;
    case types::KiCadObjectType::KOT_SCH_BUS_WIRE_ENTRY:    return SCH_BUS_WIRE_ENTRY_T;
    case types::KiCadObjectType::KOT_SCH_BUS_BUS_ENTRY:     return SCH_BUS_BUS_ENTRY_T;
    case types::KiCadObjectType::KOT_SCH_LINE:              return SCH_LINE_T;
    case types::KiCadObjectType::KOT_SCH_SHAPE:             return SCH_SHAPE_T;
    case types::KiCadObjectType::KOT_SCH_BITMAP:            return SCH_BITMAP_T;
    case types::KiCadObjectType::KOT_SCH_TEXTBOX:           return SCH_TEXTBOX_T;
    case types::KiCadObjectType::KOT_SCH_TEXT:              return SCH_TEXT_T;
    case types::KiCadObjectType::KOT_SCH_TABLE:             return SCH_TABLE_T;
    case types::KiCadObjectType::KOT_SCH_TABLECELL:         return SCH_TABLECELL_T;
    case types::KiCadObjectType::KOT_SCH_LABEL:             return SCH_LABEL_T;
    case types::KiCadObjectType::KOT_SCH_GLOBAL_LABEL:      return SCH_GLOBAL_LABEL_T;
    case types::KiCadObjectType::KOT_SCH_HIER_LABEL:        return SCH_HIER_LABEL_T;
    case types::KiCadObjectType::KOT_SCH_DIRECTIVE_LABEL:   return SCH_DIRECTIVE_LABEL_T;
    case types::KiCadObjectType::KOT_SCH_FIELD:             return SCH_FIELD_T;
    case types::KiCadObjectType::KOT_SCH_SYMBOL:            return SCH_SYMBOL_T;
    case types::KiCadObjectType::KOT_SCH_SHEET_PIN:         return SCH_SHEET_PIN_T;
    case types::KiCadObjectType::KOT_SCH_SHEET:             return SCH_SHEET_T;
    case types::KiCadObjectType::KOT_SCH_PIN:               return SCH_PIN_T;
    case types::KiCadObjectType::KOT_LIB_SYMBOL:            return LIB_SYMBOL_T;
    case types::KiCadObjectType::KOT_WSG_LINE:              return WSG_LINE_T;
    case types::KiCadObjectType::KOT_WSG_RECT:              return WSG_RECT_T;
    case types::KiCadObjectType::KOT_WSG_POLY:              return WSG_POLY_T;
    case types::KiCadObjectType::KOT_WSG_TEXT:              return WSG_TEXT_T;
    case types::KiCadObjectType::KOT_WSG_BITMAP:            return WSG_BITMAP_T;
    case types::KiCadObjectType::KOT_WSG_PAGE:              return WSG_PAGE_T;

    case types::KiCadObjectType::KOT_UNKNOWN:               return TYPE_NOT_INIT;
    default:
        wxCHECK_MSG( false, TYPE_NOT_INIT,
                     "Unhandled case in FromProtoEnum<types::KiCadObjectType>" );
    }
}


template<>
types::KiCadObjectType ToProtoEnum( KICAD_T aValue )
{
    switch( aValue )
    {
    case PCB_FOOTPRINT_T:        return types::KiCadObjectType::KOT_PCB_FOOTPRINT;
    case PCB_PAD_T:              return types::KiCadObjectType::KOT_PCB_PAD;
    case PCB_SHAPE_T:            return types::KiCadObjectType::KOT_PCB_SHAPE;
    case PCB_BARCODE_T:          return types::KiCadObjectType::KOT_PCB_BARCODE;
    case PCB_REFERENCE_IMAGE_T:  return types::KiCadObjectType::KOT_PCB_REFERENCE_IMAGE;
    case PCB_FIELD_T:            return types::KiCadObjectType::KOT_PCB_FIELD;
    case PCB_GENERATOR_T:        return types::KiCadObjectType::KOT_PCB_GENERATOR;
    case PCB_TEXT_T:             return types::KiCadObjectType::KOT_PCB_TEXT;
    case PCB_TEXTBOX_T:          return types::KiCadObjectType::KOT_PCB_TEXTBOX;
    case PCB_TABLE_T:            return types::KiCadObjectType::KOT_PCB_TABLE;
    case PCB_TABLECELL_T:        return types::KiCadObjectType::KOT_PCB_TABLECELL;
    case PCB_TRACE_T:            return types::KiCadObjectType::KOT_PCB_TRACE;
    case PCB_VIA_T:              return types::KiCadObjectType::KOT_PCB_VIA;
    case PCB_ARC_T:              return types::KiCadObjectType::KOT_PCB_ARC;
    case PCB_MARKER_T:           return types::KiCadObjectType::KOT_PCB_MARKER;
    case PCB_DIMENSION_T:        return types::KiCadObjectType::KOT_PCB_DIMENSION;
    case PCB_ZONE_T:             return types::KiCadObjectType::KOT_PCB_ZONE;
    case PCB_GROUP_T:            return types::KiCadObjectType::KOT_PCB_GROUP;
    case SCH_MARKER_T:           return types::KiCadObjectType::KOT_SCH_MARKER;
    case SCH_JUNCTION_T:         return types::KiCadObjectType::KOT_SCH_JUNCTION;
    case SCH_NO_CONNECT_T:       return types::KiCadObjectType::KOT_SCH_NO_CONNECT;
    case SCH_BUS_WIRE_ENTRY_T:   return types::KiCadObjectType::KOT_SCH_BUS_WIRE_ENTRY;
    case SCH_BUS_BUS_ENTRY_T:    return types::KiCadObjectType::KOT_SCH_BUS_BUS_ENTRY;
    case SCH_LINE_T:             return types::KiCadObjectType::KOT_SCH_LINE;
    case SCH_SHAPE_T:            return types::KiCadObjectType::KOT_SCH_SHAPE;
    case SCH_BITMAP_T:           return types::KiCadObjectType::KOT_SCH_BITMAP;
    case SCH_TEXTBOX_T:          return types::KiCadObjectType::KOT_SCH_TEXTBOX;
    case SCH_TEXT_T:             return types::KiCadObjectType::KOT_SCH_TEXT;
    case SCH_TABLE_T:            return types::KiCadObjectType::KOT_SCH_TABLE;
    case SCH_TABLECELL_T:        return types::KiCadObjectType::KOT_SCH_TABLECELL;
    case SCH_LABEL_T:            return types::KiCadObjectType::KOT_SCH_LABEL;
    case SCH_GLOBAL_LABEL_T:     return types::KiCadObjectType::KOT_SCH_GLOBAL_LABEL;
    case SCH_GROUP_T:            return types::KiCadObjectType::KOT_SCH_GROUP;
    case SCH_HIER_LABEL_T:       return types::KiCadObjectType::KOT_SCH_HIER_LABEL;
    case SCH_DIRECTIVE_LABEL_T:  return types::KiCadObjectType::KOT_SCH_DIRECTIVE_LABEL;
    case SCH_FIELD_T:            return types::KiCadObjectType::KOT_SCH_FIELD;
    case SCH_SYMBOL_T:           return types::KiCadObjectType::KOT_SCH_SYMBOL;
    case SCH_SHEET_PIN_T:        return types::KiCadObjectType::KOT_SCH_SHEET_PIN;
    case SCH_SHEET_T:            return types::KiCadObjectType::KOT_SCH_SHEET;
    case SCH_PIN_T:              return types::KiCadObjectType::KOT_SCH_PIN;
    case LIB_SYMBOL_T:           return types::KiCadObjectType::KOT_LIB_SYMBOL;
    case WSG_LINE_T:             return types::KiCadObjectType::KOT_WSG_LINE;
    case WSG_RECT_T:             return types::KiCadObjectType::KOT_WSG_RECT;
    case WSG_POLY_T:             return types::KiCadObjectType::KOT_WSG_POLY;
    case WSG_TEXT_T:             return types::KiCadObjectType::KOT_WSG_TEXT;
    case WSG_BITMAP_T:           return types::KiCadObjectType::KOT_WSG_BITMAP;
    case WSG_PAGE_T:             return types::KiCadObjectType::KOT_WSG_PAGE;
    default:
        wxCHECK_MSG( false, types::KiCadObjectType::KOT_UNKNOWN,
                     "Unhandled case in ToProtoEnum<KICAD_T>");
    }
}


template<>
PCB_LAYER_ID FromProtoEnum( board::types::BoardLayer aValue )
{
    switch( aValue )
    {
    case board::types::BoardLayer::BL_UNDEFINED:   return UNDEFINED_LAYER;
    case board::types::BoardLayer::BL_UNSELECTED:  return UNSELECTED_LAYER;
    case board::types::BoardLayer::BL_F_Cu:        return F_Cu;
    case board::types::BoardLayer::BL_In1_Cu:      return In1_Cu;
    case board::types::BoardLayer::BL_In2_Cu:      return In2_Cu;
    case board::types::BoardLayer::BL_In3_Cu:      return In3_Cu;
    case board::types::BoardLayer::BL_In4_Cu:      return In4_Cu;
    case board::types::BoardLayer::BL_In5_Cu:      return In5_Cu;
    case board::types::BoardLayer::BL_In6_Cu:      return In6_Cu;
    case board::types::BoardLayer::BL_In7_Cu:      return In7_Cu;
    case board::types::BoardLayer::BL_In8_Cu:      return In8_Cu;
    case board::types::BoardLayer::BL_In9_Cu:      return In9_Cu;
    case board::types::BoardLayer::BL_In10_Cu:     return In10_Cu;
    case board::types::BoardLayer::BL_In11_Cu:     return In11_Cu;
    case board::types::BoardLayer::BL_In12_Cu:     return In12_Cu;
    case board::types::BoardLayer::BL_In13_Cu:     return In13_Cu;
    case board::types::BoardLayer::BL_In14_Cu:     return In14_Cu;
    case board::types::BoardLayer::BL_In15_Cu:     return In15_Cu;
    case board::types::BoardLayer::BL_In16_Cu:     return In16_Cu;
    case board::types::BoardLayer::BL_In17_Cu:     return In17_Cu;
    case board::types::BoardLayer::BL_In18_Cu:     return In18_Cu;
    case board::types::BoardLayer::BL_In19_Cu:     return In19_Cu;
    case board::types::BoardLayer::BL_In20_Cu:     return In20_Cu;
    case board::types::BoardLayer::BL_In21_Cu:     return In21_Cu;
    case board::types::BoardLayer::BL_In22_Cu:     return In22_Cu;
    case board::types::BoardLayer::BL_In23_Cu:     return In23_Cu;
    case board::types::BoardLayer::BL_In24_Cu:     return In24_Cu;
    case board::types::BoardLayer::BL_In25_Cu:     return In25_Cu;
    case board::types::BoardLayer::BL_In26_Cu:     return In26_Cu;
    case board::types::BoardLayer::BL_In27_Cu:     return In27_Cu;
    case board::types::BoardLayer::BL_In28_Cu:     return In28_Cu;
    case board::types::BoardLayer::BL_In29_Cu:     return In29_Cu;
    case board::types::BoardLayer::BL_In30_Cu:     return In30_Cu;
    case board::types::BoardLayer::BL_B_Cu:        return B_Cu;
    case board::types::BoardLayer::BL_B_Adhes:     return B_Adhes;
    case board::types::BoardLayer::BL_F_Adhes:     return F_Adhes;
    case board::types::BoardLayer::BL_B_Paste:     return B_Paste;
    case board::types::BoardLayer::BL_F_Paste:     return F_Paste;
    case board::types::BoardLayer::BL_B_SilkS:     return B_SilkS;
    case board::types::BoardLayer::BL_F_SilkS:     return F_SilkS;
    case board::types::BoardLayer::BL_B_Mask:      return B_Mask;
    case board::types::BoardLayer::BL_F_Mask:      return F_Mask;
    case board::types::BoardLayer::BL_Dwgs_User:   return Dwgs_User;
    case board::types::BoardLayer::BL_Cmts_User:   return Cmts_User;
    case board::types::BoardLayer::BL_Eco1_User:   return Eco1_User;
    case board::types::BoardLayer::BL_Eco2_User:   return Eco2_User;
    case board::types::BoardLayer::BL_Edge_Cuts:   return Edge_Cuts;
    case board::types::BoardLayer::BL_Margin:      return Margin;
    case board::types::BoardLayer::BL_B_CrtYd:     return B_CrtYd;
    case board::types::BoardLayer::BL_F_CrtYd:     return F_CrtYd;
    case board::types::BoardLayer::BL_B_Fab:       return B_Fab;
    case board::types::BoardLayer::BL_F_Fab:       return F_Fab;
    case board::types::BoardLayer::BL_User_1:      return User_1;
    case board::types::BoardLayer::BL_User_2:      return User_2;
    case board::types::BoardLayer::BL_User_3:      return User_3;
    case board::types::BoardLayer::BL_User_4:      return User_4;
    case board::types::BoardLayer::BL_User_5:      return User_5;
    case board::types::BoardLayer::BL_User_6:      return User_6;
    case board::types::BoardLayer::BL_User_7:      return User_7;
    case board::types::BoardLayer::BL_User_8:      return User_8;
    case board::types::BoardLayer::BL_User_9:      return User_9;
    case board::types::BoardLayer::BL_Rescue:      return Rescue;
    case board::types::BoardLayer::BL_User_10:     return User_10;
    case board::types::BoardLayer::BL_User_11:     return User_11;
    case board::types::BoardLayer::BL_User_12:     return User_12;
    case board::types::BoardLayer::BL_User_13:     return User_13;
    case board::types::BoardLayer::BL_User_14:     return User_14;
    case board::types::BoardLayer::BL_User_15:     return User_15;
    case board::types::BoardLayer::BL_User_16:     return User_16;
    case board::types::BoardLayer::BL_User_17:     return User_17;
    case board::types::BoardLayer::BL_User_18:     return User_18;
    case board::types::BoardLayer::BL_User_19:     return User_19;
    case board::types::BoardLayer::BL_User_20:     return User_20;
    case board::types::BoardLayer::BL_User_21:     return User_21;
    case board::types::BoardLayer::BL_User_22:     return User_22;
    case board::types::BoardLayer::BL_User_23:     return User_23;
    case board::types::BoardLayer::BL_User_24:     return User_24;
    case board::types::BoardLayer::BL_User_25:     return User_25;
    case board::types::BoardLayer::BL_User_26:     return User_26;
    case board::types::BoardLayer::BL_User_27:     return User_27;
    case board::types::BoardLayer::BL_User_28:     return User_28;
    case board::types::BoardLayer::BL_User_29:     return User_29;
    case board::types::BoardLayer::BL_User_30:     return User_30;
    case board::types::BoardLayer::BL_User_31:     return User_31;
    case board::types::BoardLayer::BL_User_32:     return User_32;
    case board::types::BoardLayer::BL_User_33:     return User_33;
    case board::types::BoardLayer::BL_User_34:     return User_34;
    case board::types::BoardLayer::BL_User_35:     return User_35;
    case board::types::BoardLayer::BL_User_36:     return User_36;
    case board::types::BoardLayer::BL_User_37:     return User_37;
    case board::types::BoardLayer::BL_User_38:     return User_38;
    case board::types::BoardLayer::BL_User_39:     return User_39;
    case board::types::BoardLayer::BL_User_40:     return User_40;
    case board::types::BoardLayer::BL_User_41:     return User_41;
    case board::types::BoardLayer::BL_User_42:     return User_42;
    case board::types::BoardLayer::BL_User_43:     return User_43;
    case board::types::BoardLayer::BL_User_44:     return User_44;
    case board::types::BoardLayer::BL_User_45:     return User_45;

    case board::types::BoardLayer::BL_UNKNOWN:     return UNDEFINED_LAYER;
    default:
        wxCHECK_MSG( false, UNDEFINED_LAYER,
                     "Unhandled case in FromProtoEnum<board::types::BoardLayer>" );
    }
}


template<>
board::types::BoardLayer ToProtoEnum( PCB_LAYER_ID aValue )
{
    switch( aValue )
    {
    case UNDEFINED_LAYER:   return board::types::BoardLayer::BL_UNDEFINED;
    case UNSELECTED_LAYER:  return board::types::BoardLayer::BL_UNSELECTED;
    case F_Cu:              return board::types::BoardLayer::BL_F_Cu;
    case In1_Cu:            return board::types::BoardLayer::BL_In1_Cu;
    case In2_Cu:            return board::types::BoardLayer::BL_In2_Cu;
    case In3_Cu:            return board::types::BoardLayer::BL_In3_Cu;
    case In4_Cu:            return board::types::BoardLayer::BL_In4_Cu;
    case In5_Cu:            return board::types::BoardLayer::BL_In5_Cu;
    case In6_Cu:            return board::types::BoardLayer::BL_In6_Cu;
    case In7_Cu:            return board::types::BoardLayer::BL_In7_Cu;
    case In8_Cu:            return board::types::BoardLayer::BL_In8_Cu;
    case In9_Cu:            return board::types::BoardLayer::BL_In9_Cu;
    case In10_Cu:           return board::types::BoardLayer::BL_In10_Cu;
    case In11_Cu:           return board::types::BoardLayer::BL_In11_Cu;
    case In12_Cu:           return board::types::BoardLayer::BL_In12_Cu;
    case In13_Cu:           return board::types::BoardLayer::BL_In13_Cu;
    case In14_Cu:           return board::types::BoardLayer::BL_In14_Cu;
    case In15_Cu:           return board::types::BoardLayer::BL_In15_Cu;
    case In16_Cu:           return board::types::BoardLayer::BL_In16_Cu;
    case In17_Cu:           return board::types::BoardLayer::BL_In17_Cu;
    case In18_Cu:           return board::types::BoardLayer::BL_In18_Cu;
    case In19_Cu:           return board::types::BoardLayer::BL_In19_Cu;
    case In20_Cu:           return board::types::BoardLayer::BL_In20_Cu;
    case In21_Cu:           return board::types::BoardLayer::BL_In21_Cu;
    case In22_Cu:           return board::types::BoardLayer::BL_In22_Cu;
    case In23_Cu:           return board::types::BoardLayer::BL_In23_Cu;
    case In24_Cu:           return board::types::BoardLayer::BL_In24_Cu;
    case In25_Cu:           return board::types::BoardLayer::BL_In25_Cu;
    case In26_Cu:           return board::types::BoardLayer::BL_In26_Cu;
    case In27_Cu:           return board::types::BoardLayer::BL_In27_Cu;
    case In28_Cu:           return board::types::BoardLayer::BL_In28_Cu;
    case In29_Cu:           return board::types::BoardLayer::BL_In29_Cu;
    case In30_Cu:           return board::types::BoardLayer::BL_In30_Cu;
    case B_Cu:              return board::types::BoardLayer::BL_B_Cu;
    case B_Adhes:           return board::types::BoardLayer::BL_B_Adhes;
    case F_Adhes:           return board::types::BoardLayer::BL_F_Adhes;
    case B_Paste:           return board::types::BoardLayer::BL_B_Paste;
    case F_Paste:           return board::types::BoardLayer::BL_F_Paste;
    case B_SilkS:           return board::types::BoardLayer::BL_B_SilkS;
    case F_SilkS:           return board::types::BoardLayer::BL_F_SilkS;
    case B_Mask:            return board::types::BoardLayer::BL_B_Mask;
    case F_Mask:            return board::types::BoardLayer::BL_F_Mask;
    case Dwgs_User:         return board::types::BoardLayer::BL_Dwgs_User;
    case Cmts_User:         return board::types::BoardLayer::BL_Cmts_User;
    case Eco1_User:         return board::types::BoardLayer::BL_Eco1_User;
    case Eco2_User:         return board::types::BoardLayer::BL_Eco2_User;
    case Edge_Cuts:         return board::types::BoardLayer::BL_Edge_Cuts;
    case Margin:            return board::types::BoardLayer::BL_Margin;
    case B_CrtYd:           return board::types::BoardLayer::BL_B_CrtYd;
    case F_CrtYd:           return board::types::BoardLayer::BL_F_CrtYd;
    case B_Fab:             return board::types::BoardLayer::BL_B_Fab;
    case F_Fab:             return board::types::BoardLayer::BL_F_Fab;
    case User_1:            return board::types::BoardLayer::BL_User_1;
    case User_2:            return board::types::BoardLayer::BL_User_2;
    case User_3:            return board::types::BoardLayer::BL_User_3;
    case User_4:            return board::types::BoardLayer::BL_User_4;
    case User_5:            return board::types::BoardLayer::BL_User_5;
    case User_6:            return board::types::BoardLayer::BL_User_6;
    case User_7:            return board::types::BoardLayer::BL_User_7;
    case User_8:            return board::types::BoardLayer::BL_User_8;
    case User_9:            return board::types::BoardLayer::BL_User_9;
    case Rescue:            return board::types::BoardLayer::BL_Rescue;
    case User_10:           return board::types::BoardLayer::BL_User_10;
    case User_11:           return board::types::BoardLayer::BL_User_11;
    case User_12:           return board::types::BoardLayer::BL_User_12;
    case User_13:           return board::types::BoardLayer::BL_User_13;
    case User_14:           return board::types::BoardLayer::BL_User_14;
    case User_15:           return board::types::BoardLayer::BL_User_15;
    case User_16:           return board::types::BoardLayer::BL_User_16;
    case User_17:           return board::types::BoardLayer::BL_User_17;
    case User_18:           return board::types::BoardLayer::BL_User_18;
    case User_19:           return board::types::BoardLayer::BL_User_19;
    case User_20:           return board::types::BoardLayer::BL_User_20;
    case User_21:           return board::types::BoardLayer::BL_User_21;
    case User_22:           return board::types::BoardLayer::BL_User_22;
    case User_23:           return board::types::BoardLayer::BL_User_23;
    case User_24:           return board::types::BoardLayer::BL_User_24;
    case User_25:           return board::types::BoardLayer::BL_User_25;
    case User_26:           return board::types::BoardLayer::BL_User_26;
    case User_27:           return board::types::BoardLayer::BL_User_27;
    case User_28:           return board::types::BoardLayer::BL_User_28;
    case User_29:           return board::types::BoardLayer::BL_User_29;
    case User_30:           return board::types::BoardLayer::BL_User_30;
    case User_31:           return board::types::BoardLayer::BL_User_31;
    case User_32:           return board::types::BoardLayer::BL_User_32;
    case User_33:           return board::types::BoardLayer::BL_User_33;
    case User_34:           return board::types::BoardLayer::BL_User_34;
    case User_35:           return board::types::BoardLayer::BL_User_35;
    case User_36:           return board::types::BoardLayer::BL_User_36;
    case User_37:           return board::types::BoardLayer::BL_User_37;
    case User_38:           return board::types::BoardLayer::BL_User_38;
    case User_39:           return board::types::BoardLayer::BL_User_39;
    case User_40:           return board::types::BoardLayer::BL_User_40;
    case User_41:           return board::types::BoardLayer::BL_User_41;
    case User_42:           return board::types::BoardLayer::BL_User_42;
    case User_43:           return board::types::BoardLayer::BL_User_43;
    case User_44:           return board::types::BoardLayer::BL_User_44;
    case User_45:           return board::types::BoardLayer::BL_User_45;
    default:
        wxCHECK_MSG( false, board::types::BoardLayer::BL_UNKNOWN,
                     "Unhandled case in ToProtoEnum<PCB_LAYER_ID>");
    }
}


template<>
SCH_LAYER_ID FromProtoEnum( schematic::types::SchematicLayer aValue )
{
    switch( aValue )
    {

    default:
        wxCHECK_MSG( false, SCH_LAYER_ID_START,
                     "Unhandled case in FromProtoEnum<schematic::types::SchematicLayer>" );
    }
}


template<>
schematic::types::SchematicLayer ToProtoEnum( SCH_LAYER_ID aValue )
{
    switch( aValue )
    {

    default:
        wxCHECK_MSG( false, schematic::types::SchematicLayer::SL_UNKNOWN,
                     "Unhandled case in ToProtoEnum<SCH_LAYER_ID>");
    }
}


template<>
JOB_PAGE_SIZE FromProtoEnum( schematic::jobs::SchematicJobPageSize aValue )
{
    switch( aValue )
    {
    case schematic::jobs::SchematicJobPageSize::SJPS_AUTO: return JOB_PAGE_SIZE::PAGE_SIZE_AUTO;
    case schematic::jobs::SchematicJobPageSize::SJPS_A4:   return JOB_PAGE_SIZE::PAGE_SIZE_A4;
    case schematic::jobs::SchematicJobPageSize::SJPS_A:    return JOB_PAGE_SIZE::PAGE_SIZE_A;
    case schematic::jobs::SchematicJobPageSize::SJPS_UNKNOWN:
    default:
        wxCHECK_MSG( false, JOB_PAGE_SIZE::PAGE_SIZE_AUTO,
                     "Unhandled case in FromProtoEnum<schematic::jobs::SchematicJobPageSize>" );
    }
}


template<>
schematic::jobs::SchematicJobPageSize ToProtoEnum( JOB_PAGE_SIZE aValue )
{
    switch( aValue )
    {
    case JOB_PAGE_SIZE::PAGE_SIZE_AUTO: return schematic::jobs::SchematicJobPageSize::SJPS_AUTO;
    case JOB_PAGE_SIZE::PAGE_SIZE_A4:   return schematic::jobs::SchematicJobPageSize::SJPS_A4;
    case JOB_PAGE_SIZE::PAGE_SIZE_A:    return schematic::jobs::SchematicJobPageSize::SJPS_A;
    default:
        wxCHECK_MSG( false, schematic::jobs::SchematicJobPageSize::SJPS_UNKNOWN,
                     "Unhandled case in ToProtoEnum<JOB_PAGE_SIZE>" );
    }
}


template<>
JOB_EXPORT_SCH_NETLIST::FORMAT FromProtoEnum( schematic::jobs::SchematicNetlistFormat aValue )
{
    switch( aValue )
    {
    case schematic::jobs::SchematicNetlistFormat::SNF_KICAD_XML:
        return JOB_EXPORT_SCH_NETLIST::FORMAT::KICADXML;
    case schematic::jobs::SchematicNetlistFormat::SNF_KICAD_SEXPR:
        return JOB_EXPORT_SCH_NETLIST::FORMAT::KICADSEXPR;
    case schematic::jobs::SchematicNetlistFormat::SNF_ORCAD_PCB2:
        return JOB_EXPORT_SCH_NETLIST::FORMAT::ORCADPCB2;
    case schematic::jobs::SchematicNetlistFormat::SNF_CADSTAR:
        return JOB_EXPORT_SCH_NETLIST::FORMAT::CADSTAR;
    case schematic::jobs::SchematicNetlistFormat::SNF_SPICE:
        return JOB_EXPORT_SCH_NETLIST::FORMAT::SPICE;
    case schematic::jobs::SchematicNetlistFormat::SNF_SPICE_MODEL:
        return JOB_EXPORT_SCH_NETLIST::FORMAT::SPICEMODEL;
    case schematic::jobs::SchematicNetlistFormat::SNF_PADS:
        return JOB_EXPORT_SCH_NETLIST::FORMAT::PADS;
    case schematic::jobs::SchematicNetlistFormat::SNF_ALLEGRO:
        return JOB_EXPORT_SCH_NETLIST::FORMAT::ALLEGRO;
    case schematic::jobs::SchematicNetlistFormat::SNF_UNKNOWN:
    default:
        wxCHECK_MSG( false, JOB_EXPORT_SCH_NETLIST::FORMAT::KICADXML,
                     "Unhandled case in FromProtoEnum<schematic::jobs::SchematicNetlistFormat>" );
    }
}


template<>
schematic::jobs::SchematicNetlistFormat ToProtoEnum( JOB_EXPORT_SCH_NETLIST::FORMAT aValue )
{
    switch( aValue )
    {
    case JOB_EXPORT_SCH_NETLIST::FORMAT::KICADXML:
        return schematic::jobs::SchematicNetlistFormat::SNF_KICAD_XML;
    case JOB_EXPORT_SCH_NETLIST::FORMAT::KICADSEXPR:
        return schematic::jobs::SchematicNetlistFormat::SNF_KICAD_SEXPR;
    case JOB_EXPORT_SCH_NETLIST::FORMAT::ORCADPCB2:
        return schematic::jobs::SchematicNetlistFormat::SNF_ORCAD_PCB2;
    case JOB_EXPORT_SCH_NETLIST::FORMAT::CADSTAR:
        return schematic::jobs::SchematicNetlistFormat::SNF_CADSTAR;
    case JOB_EXPORT_SCH_NETLIST::FORMAT::SPICE:
        return schematic::jobs::SchematicNetlistFormat::SNF_SPICE;
    case JOB_EXPORT_SCH_NETLIST::FORMAT::SPICEMODEL:
        return schematic::jobs::SchematicNetlistFormat::SNF_SPICE_MODEL;
    case JOB_EXPORT_SCH_NETLIST::FORMAT::PADS:
        return schematic::jobs::SchematicNetlistFormat::SNF_PADS;
    case JOB_EXPORT_SCH_NETLIST::FORMAT::ALLEGRO:
        return schematic::jobs::SchematicNetlistFormat::SNF_ALLEGRO;
    default:
        wxCHECK_MSG( false, schematic::jobs::SchematicNetlistFormat::SNF_UNKNOWN,
                     "Unhandled case in ToProtoEnum<JOB_EXPORT_SCH_NETLIST::FORMAT>" );
    }
}


template<>
GR_TEXT_H_ALIGN_T FromProtoEnum( types::HorizontalAlignment aValue )
{
    switch( aValue )
    {
    case types::HorizontalAlignment::HA_LEFT:          return GR_TEXT_H_ALIGN_LEFT;
    case types::HorizontalAlignment::HA_CENTER:        return GR_TEXT_H_ALIGN_CENTER;
    case types::HorizontalAlignment::HA_RIGHT:         return GR_TEXT_H_ALIGN_RIGHT;
    case types::HorizontalAlignment::HA_INDETERMINATE: return GR_TEXT_H_ALIGN_INDETERMINATE;

    case types::HorizontalAlignment::HA_UNKNOWN:       return GR_TEXT_H_ALIGN_CENTER;
    default:
        wxCHECK_MSG( false, GR_TEXT_H_ALIGN_CENTER,
                     "Unhandled case in FromProtoEnum<types::HorizontalAlignment>" );
    }
}


template<>
types::HorizontalAlignment ToProtoEnum( GR_TEXT_H_ALIGN_T aValue )
{
    switch( aValue )
    {
    case GR_TEXT_H_ALIGN_LEFT:          return types::HorizontalAlignment::HA_LEFT;
    case GR_TEXT_H_ALIGN_CENTER:        return types::HorizontalAlignment::HA_CENTER;
    case GR_TEXT_H_ALIGN_RIGHT:         return types::HorizontalAlignment::HA_RIGHT;
    case GR_TEXT_H_ALIGN_INDETERMINATE: return types::HorizontalAlignment::HA_INDETERMINATE;
    default:
        wxCHECK_MSG( false, types::HorizontalAlignment::HA_UNKNOWN,
                     "Unhandled case in ToProtoEnum<GR_TEXT_H_ALIGN_T>");
    }
}


template<>
GR_TEXT_V_ALIGN_T FromProtoEnum( types::VerticalAlignment aValue )
{
    switch( aValue )
    {
    case types::VerticalAlignment::VA_TOP:           return GR_TEXT_V_ALIGN_TOP;
    case types::VerticalAlignment::VA_CENTER:        return GR_TEXT_V_ALIGN_CENTER;
    case types::VerticalAlignment::VA_BOTTOM:        return GR_TEXT_V_ALIGN_BOTTOM;
    case types::VerticalAlignment::VA_INDETERMINATE: return GR_TEXT_V_ALIGN_INDETERMINATE;

    case types::VerticalAlignment::VA_UNKNOWN:       return GR_TEXT_V_ALIGN_CENTER;
    default:
        wxCHECK_MSG( false, GR_TEXT_V_ALIGN_CENTER,
                     "Unhandled case in FromProtoEnum<types::VerticalAlignment>" );
    }
}


template<>
types::VerticalAlignment ToProtoEnum( GR_TEXT_V_ALIGN_T aValue )
{
    switch( aValue )
    {
    case GR_TEXT_V_ALIGN_TOP:           return types::VerticalAlignment::VA_TOP;
    case GR_TEXT_V_ALIGN_CENTER:        return types::VerticalAlignment::VA_CENTER;
    case GR_TEXT_V_ALIGN_BOTTOM:        return types::VerticalAlignment::VA_BOTTOM;
    case GR_TEXT_V_ALIGN_INDETERMINATE: return types::VerticalAlignment::VA_INDETERMINATE;
    default:
        wxCHECK_MSG( false, types::VerticalAlignment::VA_UNKNOWN,
                     "Unhandled case in ToProtoEnum<GR_TEXT_V_ALIGN_T>");
    }
}


template<>
LINE_STYLE FromProtoEnum( types::StrokeLineStyle aValue )
{
    switch( aValue )
    {
    case types::StrokeLineStyle::SLS_DEFAULT:    return LINE_STYLE::DEFAULT;
    case types::StrokeLineStyle::SLS_SOLID:      return LINE_STYLE::SOLID;
    case types::StrokeLineStyle::SLS_DASH:       return LINE_STYLE::DASH;
    case types::StrokeLineStyle::SLS_DOT:        return LINE_STYLE::DOT;
    case types::StrokeLineStyle::SLS_DASHDOT:    return LINE_STYLE::DASHDOT;
    case types::StrokeLineStyle::SLS_DASHDOTDOT: return LINE_STYLE::DASHDOTDOT;
    case types::StrokeLineStyle::SLS_UNKNOWN:
    default:
        wxCHECK_MSG( false, LINE_STYLE::DEFAULT,
                     "Unhandled case in FromProtoEnum<types::StrokeLineStyle>" );
    }
}


template<>
types::StrokeLineStyle ToProtoEnum( LINE_STYLE aValue )
{
    switch( aValue )
    {
    case LINE_STYLE::DEFAULT:    return types::StrokeLineStyle::SLS_DEFAULT;
    case LINE_STYLE::SOLID:      return types::StrokeLineStyle::SLS_SOLID;
    case LINE_STYLE::DASH:       return types::StrokeLineStyle::SLS_DASH;
    case LINE_STYLE::DOT:        return types::StrokeLineStyle::SLS_DOT;
    case LINE_STYLE::DASHDOT:    return types::StrokeLineStyle::SLS_DASHDOT;
    case LINE_STYLE::DASHDOTDOT: return types::StrokeLineStyle::SLS_DASHDOTDOT;
    default:
        wxCHECK_MSG( false, types::StrokeLineStyle::SLS_UNKNOWN,
                     "Unhandled case in ToProtoEnum<LINE_STYLE>");
    }
}


template<>
ELECTRICAL_PINTYPE FromProtoEnum( types::ElectricalPinType aValue )
{
    switch( aValue )
    {
    case types::ElectricalPinType::EPT_INPUT:           return ELECTRICAL_PINTYPE::PT_INPUT;
    case types::ElectricalPinType::EPT_OUTPUT:          return ELECTRICAL_PINTYPE::PT_OUTPUT;
    case types::ElectricalPinType::EPT_BIDIRECTIONAL:   return ELECTRICAL_PINTYPE::PT_BIDI;
    case types::ElectricalPinType::EPT_TRISTATE:        return ELECTRICAL_PINTYPE::PT_TRISTATE;
    case types::ElectricalPinType::EPT_PASSIVE:         return ELECTRICAL_PINTYPE::PT_PASSIVE;
    case types::ElectricalPinType::EPT_FREE:            return ELECTRICAL_PINTYPE::PT_NIC;
    case types::ElectricalPinType::EPT_UNSPECIFIED:     return ELECTRICAL_PINTYPE::PT_UNSPECIFIED;
    case types::ElectricalPinType::EPT_POWER_INPUT:     return ELECTRICAL_PINTYPE::PT_POWER_IN;
    case types::ElectricalPinType::EPT_POWER_OUTPUT:    return ELECTRICAL_PINTYPE::PT_POWER_OUT;
    case types::ElectricalPinType::EPT_OPEN_COLLECTOR:  return ELECTRICAL_PINTYPE::PT_OPENCOLLECTOR;
    case types::ElectricalPinType::EPT_OPEN_EMITTER:    return ELECTRICAL_PINTYPE::PT_OPENEMITTER;
    case types::ElectricalPinType::EPT_NO_CONNECT:      return ELECTRICAL_PINTYPE::PT_NC;
    case types::ElectricalPinType::EPT_UNKNOWN:
    default:
        wxCHECK_MSG( false, ELECTRICAL_PINTYPE::PT_UNSPECIFIED,
                     "Unhandled case in FromProtoEnum<types::ElectricalPinType>" );
    }
}


template<>
types::ElectricalPinType ToProtoEnum( ELECTRICAL_PINTYPE aValue )
{
    switch( aValue )
    {
    case ELECTRICAL_PINTYPE::PT_INPUT:         return types::ElectricalPinType::EPT_INPUT;
    case ELECTRICAL_PINTYPE::PT_OUTPUT:        return types::ElectricalPinType::EPT_OUTPUT;
    case ELECTRICAL_PINTYPE::PT_BIDI:          return types::ElectricalPinType::EPT_BIDIRECTIONAL;
    case ELECTRICAL_PINTYPE::PT_TRISTATE:      return types::ElectricalPinType::EPT_TRISTATE;
    case ELECTRICAL_PINTYPE::PT_PASSIVE:       return types::ElectricalPinType::EPT_PASSIVE;
    case ELECTRICAL_PINTYPE::PT_NIC:           return types::ElectricalPinType::EPT_FREE;
    case ELECTRICAL_PINTYPE::PT_UNSPECIFIED:   return types::ElectricalPinType::EPT_UNSPECIFIED;
    case ELECTRICAL_PINTYPE::PT_POWER_IN:      return types::ElectricalPinType::EPT_POWER_INPUT;
    case ELECTRICAL_PINTYPE::PT_POWER_OUT:     return types::ElectricalPinType::EPT_POWER_OUTPUT;
    case ELECTRICAL_PINTYPE::PT_OPENCOLLECTOR: return types::ElectricalPinType::EPT_OPEN_COLLECTOR;
    case ELECTRICAL_PINTYPE::PT_OPENEMITTER:   return types::ElectricalPinType::EPT_OPEN_EMITTER;
    case ELECTRICAL_PINTYPE::PT_NC:            return types::ElectricalPinType::EPT_NO_CONNECT;
    // Inherit shouldn't be serialized, it's an internal flag
    case ELECTRICAL_PINTYPE::PT_INHERIT:
    default:
        wxCHECK_MSG( false, types::ElectricalPinType::EPT_UNKNOWN,
                     "Unhandled case in ToProtoEnum<ELECTRICAL_PINTYPE>");
    }
}


template<>
types::RuleSeverity ToProtoEnum( SEVERITY aValue )
{
    switch( aValue )
    {
    case RPT_SEVERITY_WARNING:   return types::RuleSeverity::RS_WARNING;
    case RPT_SEVERITY_ERROR:     return types::RuleSeverity::RS_ERROR;
    case RPT_SEVERITY_EXCLUSION: return types::RuleSeverity::RS_EXCLUSION;
    case RPT_SEVERITY_IGNORE:    return types::RuleSeverity::RS_IGNORE;
    case RPT_SEVERITY_INFO:      return types::RuleSeverity::RS_INFO;
    case RPT_SEVERITY_ACTION:    return types::RuleSeverity::RS_ACTION;
    case RPT_SEVERITY_DEBUG:     return types::RuleSeverity::RS_DEBUG;
    case RPT_SEVERITY_UNDEFINED: return types::RuleSeverity::RS_UNDEFINED;
    default:
        wxCHECK_MSG( false, types::RuleSeverity::RS_UNDEFINED,
                     "Unhandled case in ToProtoEnum<SEVERITY>");
    }
}


template<>
SEVERITY FromProtoEnum( types::RuleSeverity aValue )
{
    switch( aValue )
    {
    case types::RuleSeverity::RS_WARNING:   return RPT_SEVERITY_WARNING;
    case types::RuleSeverity::RS_ERROR:     return RPT_SEVERITY_ERROR;
    case types::RuleSeverity::RS_EXCLUSION: return RPT_SEVERITY_EXCLUSION;
    case types::RuleSeverity::RS_IGNORE:    return RPT_SEVERITY_IGNORE;
    case types::RuleSeverity::RS_INFO:      return RPT_SEVERITY_INFO;
    case types::RuleSeverity::RS_ACTION:    return RPT_SEVERITY_ACTION;
    case types::RuleSeverity::RS_DEBUG:     return RPT_SEVERITY_DEBUG;
    case types::RuleSeverity::RS_UNKNOWN:
    default:
        return RPT_SEVERITY_UNDEFINED;
    }
}


template<>
board::DrcErrorType ToProtoEnum( PCB_DRC_CODE aValue )
{
    switch( aValue )
    {
    case DRCE_UNCONNECTED_ITEMS:                return board::DrcErrorType::DRCET_UNCONNECTED_ITEMS;
    case DRCE_SHORTING_ITEMS:                   return board::DrcErrorType::DRCET_SHORTING_ITEMS;
    case DRCE_ALLOWED_ITEMS:                    return board::DrcErrorType::DRCET_ALLOWED_ITEMS;
    case DRCE_TEXT_ON_EDGECUTS:                 return board::DrcErrorType::DRCET_TEXT_ON_EDGECUTS;
    case DRCE_CLEARANCE:                        return board::DrcErrorType::DRCET_CLEARANCE;
    case DRCE_CREEPAGE:                         return board::DrcErrorType::DRCET_CREEPAGE;
    case DRCE_TRACKS_CROSSING:                  return board::DrcErrorType::DRCET_TRACKS_CROSSING;
    case DRCE_EDGE_CLEARANCE:                   return board::DrcErrorType::DRCET_EDGE_CLEARANCE;
    case DRCE_ZONES_INTERSECT:                  return board::DrcErrorType::DRCET_ZONES_INTERSECT;
    case DRCE_ISOLATED_COPPER:                  return board::DrcErrorType::DRCET_ISOLATED_COPPER;
    case DRCE_STARVED_THERMAL:                  return board::DrcErrorType::DRCET_STARVED_THERMAL;
    case DRCE_DANGLING_VIA:                     return board::DrcErrorType::DRCET_DANGLING_VIA;
    case DRCE_DANGLING_TRACK:                   return board::DrcErrorType::DRCET_DANGLING_TRACK;
    case DRCE_DRILLED_HOLES_TOO_CLOSE:          return board::DrcErrorType::DRCET_DRILLED_HOLES_TOO_CLOSE;
    case DRCE_DRILLED_HOLES_COLOCATED:          return board::DrcErrorType::DRCET_DRILLED_HOLES_COLOCATED;
    case DRCE_HOLE_CLEARANCE:                   return board::DrcErrorType::DRCET_HOLE_CLEARANCE;
    case DRCE_CONNECTION_WIDTH:                 return board::DrcErrorType::DRCET_CONNECTION_WIDTH;
    case DRCE_TRACK_WIDTH:                      return board::DrcErrorType::DRCET_TRACK_WIDTH;
    case DRCE_TRACK_ANGLE:                      return board::DrcErrorType::DRCET_TRACK_ANGLE;
    case DRCE_TRACK_SEGMENT_LENGTH:             return board::DrcErrorType::DRCET_TRACK_SEGMENT_LENGTH;
    case DRCE_ANNULAR_WIDTH:                    return board::DrcErrorType::DRCET_ANNULAR_WIDTH;
    case DRCE_DRILL_OUT_OF_RANGE:               return board::DrcErrorType::DRCET_DRILL_OUT_OF_RANGE;
    case DRCE_VIA_DIAMETER:                     return board::DrcErrorType::DRCET_VIA_DIAMETER;
    case DRCE_PADSTACK:                         return board::DrcErrorType::DRCET_PADSTACK;
    case DRCE_PADSTACK_INVALID:                 return board::DrcErrorType::DRCET_PADSTACK_INVALID;
    case DRCE_MICROVIA_DRILL_OUT_OF_RANGE:      return board::DrcErrorType::DRCET_MICROVIA_DRILL_OUT_OF_RANGE;
    case DRCE_OVERLAPPING_FOOTPRINTS:           return board::DrcErrorType::DRCET_OVERLAPPING_FOOTPRINTS;
    case DRCE_MISSING_COURTYARD:                return board::DrcErrorType::DRCET_MISSING_COURTYARD;
    case DRCE_MALFORMED_COURTYARD:              return board::DrcErrorType::DRCET_MALFORMED_COURTYARD;
    case DRCE_PTH_IN_COURTYARD:                 return board::DrcErrorType::DRCET_PTH_IN_COURTYARD;
    case DRCE_NPTH_IN_COURTYARD:                return board::DrcErrorType::DRCET_NPTH_IN_COURTYARD;
    case DRCE_DISABLED_LAYER_ITEM:              return board::DrcErrorType::DRCET_DISABLED_LAYER_ITEM;
    case DRCE_INVALID_OUTLINE:                  return board::DrcErrorType::DRCET_INVALID_OUTLINE;
    case DRCE_MISSING_FOOTPRINT:                return board::DrcErrorType::DRCET_MISSING_FOOTPRINT;
    case DRCE_DUPLICATE_FOOTPRINT:              return board::DrcErrorType::DRCET_DUPLICATE_FOOTPRINT;
    case DRCE_NET_CONFLICT:                     return board::DrcErrorType::DRCET_NET_CONFLICT;
    case DRCE_EXTRA_FOOTPRINT:                  return board::DrcErrorType::DRCET_EXTRA_FOOTPRINT;
    case DRCE_SCHEMATIC_PARITY:                 return board::DrcErrorType::DRCET_SCHEMATIC_PARITY;
    case DRCE_SCHEMATIC_FIELDS_PARITY:          return board::DrcErrorType::DRCET_SCHEMATIC_FIELDS_PARITY;
    case DRCE_FOOTPRINT_FILTERS:                return board::DrcErrorType::DRCET_FOOTPRINT_FILTERS;
    case DRCE_LIB_FOOTPRINT_ISSUES:             return board::DrcErrorType::DRCET_LIB_FOOTPRINT_ISSUES;
    case DRCE_LIB_FOOTPRINT_MISMATCH:           return board::DrcErrorType::DRCET_LIB_FOOTPRINT_MISMATCH;
    case DRCE_UNRESOLVED_VARIABLE:              return board::DrcErrorType::DRCET_UNRESOLVED_VARIABLE;
    case DRCE_ASSERTION_FAILURE:                return board::DrcErrorType::DRCET_ASSERTION_FAILURE;
    case DRCE_GENERIC_WARNING:                  return board::DrcErrorType::DRCET_GENERIC_WARNING;
    case DRCE_GENERIC_ERROR:                    return board::DrcErrorType::DRCET_GENERIC_ERROR;
    case DRCE_COPPER_SLIVER:                    return board::DrcErrorType::DRCET_COPPER_SLIVER;
    case DRCE_SILK_CLEARANCE:                   return board::DrcErrorType::DRCET_SILK_CLEARANCE;
    case DRCE_SILK_MASK_CLEARANCE:              return board::DrcErrorType::DRCET_SILK_MASK_CLEARANCE;
    case DRCE_SILK_EDGE_CLEARANCE:              return board::DrcErrorType::DRCET_SILK_EDGE_CLEARANCE;
    case DRCE_SOLDERMASK_BRIDGE:                return board::DrcErrorType::DRCET_SOLDERMASK_BRIDGE;
    case DRCE_TEXT_HEIGHT:                      return board::DrcErrorType::DRCET_TEXT_HEIGHT;
    case DRCE_TEXT_THICKNESS:                   return board::DrcErrorType::DRCET_TEXT_THICKNESS;
    case DRCE_LENGTH_OUT_OF_RANGE:              return board::DrcErrorType::DRCET_LENGTH_OUT_OF_RANGE;
    case DRCE_SKEW_OUT_OF_RANGE:                return board::DrcErrorType::DRCET_SKEW_OUT_OF_RANGE;
    case DRCE_VIA_COUNT_OUT_OF_RANGE:           return board::DrcErrorType::DRCET_VIA_COUNT_OUT_OF_RANGE;
    case DRCE_DIFF_PAIR_GAP_OUT_OF_RANGE:       return board::DrcErrorType::DRCET_DIFF_PAIR_GAP_OUT_OF_RANGE;
    case DRCE_DIFF_PAIR_UNCOUPLED_LENGTH_TOO_LONG: return board::DrcErrorType::DRCET_DIFF_PAIR_UNCOUPLED_LENGTH_TOO_LONG;
    case DRCE_FOOTPRINT:                        return board::DrcErrorType::DRCET_FOOTPRINT;
    case DRCE_FOOTPRINT_TYPE_MISMATCH:          return board::DrcErrorType::DRCET_FOOTPRINT_TYPE_MISMATCH;
    case DRCE_PAD_TH_WITH_NO_HOLE:              return board::DrcErrorType::DRCET_PAD_TH_WITH_NO_HOLE;
    case DRCE_MIRRORED_TEXT_ON_FRONT_LAYER:     return board::DrcErrorType::DRCET_MIRRORED_TEXT_ON_FRONT_LAYER;
    case DRCE_NONMIRRORED_TEXT_ON_BACK_LAYER:   return board::DrcErrorType::DRCET_NONMIRRORED_TEXT_ON_BACK_LAYER;
    case DRCE_MISSING_TUNING_PROFILE:           return board::DrcErrorType::DRCET_MISSING_TUNING_PROFILE;
    case DRCE_TRACK_ON_POST_MACHINED_LAYER:     return board::DrcErrorType::DRCET_TRACK_ON_POST_MACHINED_LAYER;
    case DRCE_TRACK_NOT_CENTERED_ON_VIA:        return board::DrcErrorType::DRCET_TRACK_NOT_CENTERED_ON_VIA;
    default:
        wxCHECK_MSG( false, board::DrcErrorType::DRCET_UNKNOWN,
                     "Unhandled case in ToProtoEnum<PCB_DRC_CODE>" );
    }
}


template<>
PCB_DRC_CODE FromProtoEnum( board::DrcErrorType aValue )
{
    switch( aValue )
    {
    case board::DrcErrorType::DRCET_UNCONNECTED_ITEMS:           return DRCE_UNCONNECTED_ITEMS;
    case board::DrcErrorType::DRCET_SHORTING_ITEMS:              return DRCE_SHORTING_ITEMS;
    case board::DrcErrorType::DRCET_ALLOWED_ITEMS:               return DRCE_ALLOWED_ITEMS;
    case board::DrcErrorType::DRCET_TEXT_ON_EDGECUTS:            return DRCE_TEXT_ON_EDGECUTS;
    case board::DrcErrorType::DRCET_CLEARANCE:                   return DRCE_CLEARANCE;
    case board::DrcErrorType::DRCET_CREEPAGE:                    return DRCE_CREEPAGE;
    case board::DrcErrorType::DRCET_TRACKS_CROSSING:             return DRCE_TRACKS_CROSSING;
    case board::DrcErrorType::DRCET_EDGE_CLEARANCE:              return DRCE_EDGE_CLEARANCE;
    case board::DrcErrorType::DRCET_ZONES_INTERSECT:             return DRCE_ZONES_INTERSECT;
    case board::DrcErrorType::DRCET_ISOLATED_COPPER:             return DRCE_ISOLATED_COPPER;
    case board::DrcErrorType::DRCET_STARVED_THERMAL:             return DRCE_STARVED_THERMAL;
    case board::DrcErrorType::DRCET_DANGLING_VIA:                return DRCE_DANGLING_VIA;
    case board::DrcErrorType::DRCET_DANGLING_TRACK:              return DRCE_DANGLING_TRACK;
    case board::DrcErrorType::DRCET_DRILLED_HOLES_TOO_CLOSE:     return DRCE_DRILLED_HOLES_TOO_CLOSE;
    case board::DrcErrorType::DRCET_DRILLED_HOLES_COLOCATED:     return DRCE_DRILLED_HOLES_COLOCATED;
    case board::DrcErrorType::DRCET_HOLE_CLEARANCE:              return DRCE_HOLE_CLEARANCE;
    case board::DrcErrorType::DRCET_CONNECTION_WIDTH:            return DRCE_CONNECTION_WIDTH;
    case board::DrcErrorType::DRCET_TRACK_WIDTH:                 return DRCE_TRACK_WIDTH;
    case board::DrcErrorType::DRCET_TRACK_ANGLE:                 return DRCE_TRACK_ANGLE;
    case board::DrcErrorType::DRCET_TRACK_SEGMENT_LENGTH:        return DRCE_TRACK_SEGMENT_LENGTH;
    case board::DrcErrorType::DRCET_ANNULAR_WIDTH:               return DRCE_ANNULAR_WIDTH;
    case board::DrcErrorType::DRCET_DRILL_OUT_OF_RANGE:          return DRCE_DRILL_OUT_OF_RANGE;
    case board::DrcErrorType::DRCET_VIA_DIAMETER:                return DRCE_VIA_DIAMETER;
    case board::DrcErrorType::DRCET_PADSTACK:                    return DRCE_PADSTACK;
    case board::DrcErrorType::DRCET_PADSTACK_INVALID:            return DRCE_PADSTACK_INVALID;
    case board::DrcErrorType::DRCET_MICROVIA_DRILL_OUT_OF_RANGE: return DRCE_MICROVIA_DRILL_OUT_OF_RANGE;
    case board::DrcErrorType::DRCET_OVERLAPPING_FOOTPRINTS:      return DRCE_OVERLAPPING_FOOTPRINTS;
    case board::DrcErrorType::DRCET_MISSING_COURTYARD:           return DRCE_MISSING_COURTYARD;
    case board::DrcErrorType::DRCET_MALFORMED_COURTYARD:         return DRCE_MALFORMED_COURTYARD;
    case board::DrcErrorType::DRCET_PTH_IN_COURTYARD:            return DRCE_PTH_IN_COURTYARD;
    case board::DrcErrorType::DRCET_NPTH_IN_COURTYARD:           return DRCE_NPTH_IN_COURTYARD;
    case board::DrcErrorType::DRCET_DISABLED_LAYER_ITEM:         return DRCE_DISABLED_LAYER_ITEM;
    case board::DrcErrorType::DRCET_INVALID_OUTLINE:             return DRCE_INVALID_OUTLINE;
    case board::DrcErrorType::DRCET_MISSING_FOOTPRINT:           return DRCE_MISSING_FOOTPRINT;
    case board::DrcErrorType::DRCET_DUPLICATE_FOOTPRINT:         return DRCE_DUPLICATE_FOOTPRINT;
    case board::DrcErrorType::DRCET_NET_CONFLICT:                return DRCE_NET_CONFLICT;
    case board::DrcErrorType::DRCET_EXTRA_FOOTPRINT:             return DRCE_EXTRA_FOOTPRINT;
    case board::DrcErrorType::DRCET_SCHEMATIC_PARITY:            return DRCE_SCHEMATIC_PARITY;
    case board::DrcErrorType::DRCET_SCHEMATIC_FIELDS_PARITY:     return DRCE_SCHEMATIC_FIELDS_PARITY;
    case board::DrcErrorType::DRCET_FOOTPRINT_FILTERS:           return DRCE_FOOTPRINT_FILTERS;
    case board::DrcErrorType::DRCET_LIB_FOOTPRINT_ISSUES:        return DRCE_LIB_FOOTPRINT_ISSUES;
    case board::DrcErrorType::DRCET_LIB_FOOTPRINT_MISMATCH:      return DRCE_LIB_FOOTPRINT_MISMATCH;
    case board::DrcErrorType::DRCET_UNRESOLVED_VARIABLE:         return DRCE_UNRESOLVED_VARIABLE;
    case board::DrcErrorType::DRCET_ASSERTION_FAILURE:           return DRCE_ASSERTION_FAILURE;
    case board::DrcErrorType::DRCET_GENERIC_WARNING:             return DRCE_GENERIC_WARNING;
    case board::DrcErrorType::DRCET_GENERIC_ERROR:               return DRCE_GENERIC_ERROR;
    case board::DrcErrorType::DRCET_COPPER_SLIVER:               return DRCE_COPPER_SLIVER;
    case board::DrcErrorType::DRCET_SILK_CLEARANCE:              return DRCE_SILK_CLEARANCE;
    case board::DrcErrorType::DRCET_SILK_MASK_CLEARANCE:         return DRCE_SILK_MASK_CLEARANCE;
    case board::DrcErrorType::DRCET_SILK_EDGE_CLEARANCE:         return DRCE_SILK_EDGE_CLEARANCE;
    case board::DrcErrorType::DRCET_SOLDERMASK_BRIDGE:           return DRCE_SOLDERMASK_BRIDGE;
    case board::DrcErrorType::DRCET_TEXT_HEIGHT:                 return DRCE_TEXT_HEIGHT;
    case board::DrcErrorType::DRCET_TEXT_THICKNESS:              return DRCE_TEXT_THICKNESS;
    case board::DrcErrorType::DRCET_LENGTH_OUT_OF_RANGE:         return DRCE_LENGTH_OUT_OF_RANGE;
    case board::DrcErrorType::DRCET_SKEW_OUT_OF_RANGE:           return DRCE_SKEW_OUT_OF_RANGE;
    case board::DrcErrorType::DRCET_VIA_COUNT_OUT_OF_RANGE:      return DRCE_VIA_COUNT_OUT_OF_RANGE;
    case board::DrcErrorType::DRCET_DIFF_PAIR_GAP_OUT_OF_RANGE:  return DRCE_DIFF_PAIR_GAP_OUT_OF_RANGE;
    case board::DrcErrorType::DRCET_DIFF_PAIR_UNCOUPLED_LENGTH_TOO_LONG: return DRCE_DIFF_PAIR_UNCOUPLED_LENGTH_TOO_LONG;
    case board::DrcErrorType::DRCET_FOOTPRINT:                      return DRCE_FOOTPRINT;
    case board::DrcErrorType::DRCET_FOOTPRINT_TYPE_MISMATCH:        return DRCE_FOOTPRINT_TYPE_MISMATCH;
    case board::DrcErrorType::DRCET_PAD_TH_WITH_NO_HOLE:            return DRCE_PAD_TH_WITH_NO_HOLE;
    case board::DrcErrorType::DRCET_MIRRORED_TEXT_ON_FRONT_LAYER:   return DRCE_MIRRORED_TEXT_ON_FRONT_LAYER;
    case board::DrcErrorType::DRCET_NONMIRRORED_TEXT_ON_BACK_LAYER: return DRCE_NONMIRRORED_TEXT_ON_BACK_LAYER;
    case board::DrcErrorType::DRCET_MISSING_TUNING_PROFILE:         return DRCE_MISSING_TUNING_PROFILE;
    case board::DrcErrorType::DRCET_TRACK_ON_POST_MACHINED_LAYER:   return DRCE_TRACK_ON_POST_MACHINED_LAYER;
    case board::DrcErrorType::DRCET_TRACK_NOT_CENTERED_ON_VIA:      return DRCE_TRACK_NOT_CENTERED_ON_VIA;

    case board::DrcErrorType::DRCET_UNKNOWN:
    default:
        return static_cast<PCB_DRC_CODE>( 0 );
    }
}
