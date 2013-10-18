#include <hxcpp.h>

#ifndef INCLUDED_coopy_CellInfo
#include <coopy/CellInfo.h>
#endif
#ifndef INCLUDED_coopy_DiffRender
#include <coopy/DiffRender.h>
#endif
#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_TableText
#include <coopy/TableText.h>
#endif
namespace coopy{

Void DiffRender_obj::__construct()
{
HX_STACK_FRAME("coopy.DiffRender","new",0x9d1184ef,"coopy.DiffRender.new","coopy/DiffRender.hx",13,0xe6f800c0)
{
	HX_STACK_LINE(14)
	this->text_to_insert = Array_obj< ::String >::__new();
	HX_STACK_LINE(15)
	this->open = false;
	HX_STACK_LINE(16)
	this->pretty_arrows = true;
}
;
	return null();
}

DiffRender_obj::~DiffRender_obj() { }

Dynamic DiffRender_obj::__CreateEmpty() { return  new DiffRender_obj; }
hx::ObjectPtr< DiffRender_obj > DiffRender_obj::__new()
{  hx::ObjectPtr< DiffRender_obj > result = new DiffRender_obj();
	result->__construct();
	return result;}

Dynamic DiffRender_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< DiffRender_obj > result = new DiffRender_obj();
	result->__construct();
	return result;}

Void DiffRender_obj::usePrettyArrows( bool flag){
{
		HX_STACK_FRAME("coopy.DiffRender","usePrettyArrows",0x83830fd6,"coopy.DiffRender.usePrettyArrows","coopy/DiffRender.hx",20,0xe6f800c0)
		HX_STACK_THIS(this)
		HX_STACK_ARG(flag,"flag")
		HX_STACK_LINE(20)
		this->pretty_arrows = flag;
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC1(DiffRender_obj,usePrettyArrows,(void))

Void DiffRender_obj::insert( ::String str){
{
		HX_STACK_FRAME("coopy.DiffRender","insert",0x2e2d920a,"coopy.DiffRender.insert","coopy/DiffRender.hx",24,0xe6f800c0)
		HX_STACK_THIS(this)
		HX_STACK_ARG(str,"str")
		HX_STACK_LINE(24)
		this->text_to_insert->push(str);
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC1(DiffRender_obj,insert,(void))

Void DiffRender_obj::beginTable( ){
{
		HX_STACK_FRAME("coopy.DiffRender","beginTable",0x10fb5476,"coopy.DiffRender.beginTable","coopy/DiffRender.hx",28,0xe6f800c0)
		HX_STACK_THIS(this)
		HX_STACK_LINE(28)
		this->insert(HX_CSTRING("<table>\n"));
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(DiffRender_obj,beginTable,(void))

Void DiffRender_obj::beginRow( ::String mode){
{
		HX_STACK_FRAME("coopy.DiffRender","beginRow",0xba2f1482,"coopy.DiffRender.beginRow","coopy/DiffRender.hx",31,0xe6f800c0)
		HX_STACK_THIS(this)
		HX_STACK_ARG(mode,"mode")
		HX_STACK_LINE(32)
		this->td_open = HX_CSTRING("<td");
		HX_STACK_LINE(33)
		this->td_close = HX_CSTRING("</td>");
		HX_STACK_LINE(34)
		::String row_class = HX_CSTRING("");		HX_STACK_VAR(row_class,"row_class");
		HX_STACK_LINE(35)
		if (((mode == HX_CSTRING("header")))){
			HX_STACK_LINE(36)
			this->td_open = HX_CSTRING("<th");
			HX_STACK_LINE(37)
			this->td_close = HX_CSTRING("</th>");
		}
		else{
			HX_STACK_LINE(39)
			row_class = mode;
		}
		HX_STACK_LINE(41)
		::String tr = HX_CSTRING("<tr>");		HX_STACK_VAR(tr,"tr");
		HX_STACK_LINE(42)
		if (((row_class != HX_CSTRING("")))){
			HX_STACK_LINE(43)
			tr = ((HX_CSTRING("<tr class=\"") + row_class) + HX_CSTRING("\">"));
		}
		HX_STACK_LINE(45)
		this->insert(tr);
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC1(DiffRender_obj,beginRow,(void))

Void DiffRender_obj::insertCell( ::String txt,::String mode){
{
		HX_STACK_FRAME("coopy.DiffRender","insertCell",0xb7f6accc,"coopy.DiffRender.insertCell","coopy/DiffRender.hx",48,0xe6f800c0)
		HX_STACK_THIS(this)
		HX_STACK_ARG(txt,"txt")
		HX_STACK_ARG(mode,"mode")
		HX_STACK_LINE(49)
		::String cell_decorate = HX_CSTRING("");		HX_STACK_VAR(cell_decorate,"cell_decorate");
		HX_STACK_LINE(50)
		if (((mode != HX_CSTRING("")))){
			HX_STACK_LINE(51)
			cell_decorate = ((HX_CSTRING(" class=\"") + mode) + HX_CSTRING("\""));
		}
		HX_STACK_LINE(53)
		this->insert(((this->td_open + cell_decorate) + HX_CSTRING(">")));
		HX_STACK_LINE(54)
		this->insert(txt);
		HX_STACK_LINE(55)
		this->insert(this->td_close);
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC2(DiffRender_obj,insertCell,(void))

Void DiffRender_obj::endRow( ){
{
		HX_STACK_FRAME("coopy.DiffRender","endRow",0x8ac77fd0,"coopy.DiffRender.endRow","coopy/DiffRender.hx",59,0xe6f800c0)
		HX_STACK_THIS(this)
		HX_STACK_LINE(59)
		this->insert(HX_CSTRING("</tr>\n"));
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(DiffRender_obj,endRow,(void))

Void DiffRender_obj::endTable( ){
{
		HX_STACK_FRAME("coopy.DiffRender","endTable",0x88ffaf44,"coopy.DiffRender.endTable","coopy/DiffRender.hx",63,0xe6f800c0)
		HX_STACK_THIS(this)
		HX_STACK_LINE(63)
		this->insert(HX_CSTRING("</table>\n"));
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(DiffRender_obj,endTable,(void))

::String DiffRender_obj::html( ){
	HX_STACK_FRAME("coopy.DiffRender","html",0xce56ddfc,"coopy.DiffRender.html","coopy/DiffRender.hx",67,0xe6f800c0)
	HX_STACK_THIS(this)
	HX_STACK_LINE(67)
	return this->text_to_insert->join(HX_CSTRING(""));
}


HX_DEFINE_DYNAMIC_FUNC0(DiffRender_obj,html,return )

::String DiffRender_obj::toString( ){
	HX_STACK_FRAME("coopy.DiffRender","toString",0xb88d35bd,"coopy.DiffRender.toString","coopy/DiffRender.hx",71,0xe6f800c0)
	HX_STACK_THIS(this)
	HX_STACK_LINE(71)
	return this->html();
}


HX_DEFINE_DYNAMIC_FUNC0(DiffRender_obj,toString,return )

Void DiffRender_obj::render( ::coopy::Table rows){
{
		HX_STACK_FRAME("coopy.DiffRender","render",0x9579ba27,"coopy.DiffRender.render","coopy/DiffRender.hx",161,0xe6f800c0)
		HX_STACK_THIS(this)
		HX_STACK_ARG(rows,"rows")
		HX_STACK_LINE(162)
		if (((bool((rows->__Field(HX_CSTRING("get_width"),true)() == (int)0)) || bool((rows->__Field(HX_CSTRING("get_height"),true)() == (int)0))))){
			HX_STACK_LINE(162)
			return null();
		}
		HX_STACK_LINE(163)
		::coopy::DiffRender render = hx::ObjectPtr<OBJ_>(this);		HX_STACK_VAR(render,"render");
		HX_STACK_LINE(164)
		render->beginTable();
		HX_STACK_LINE(165)
		int change_row = (int)-1;		HX_STACK_VAR(change_row,"change_row");
		HX_STACK_LINE(166)
		::coopy::TableText tt = ::coopy::TableText_obj::__new(rows);		HX_STACK_VAR(tt,"tt");
		HX_STACK_LINE(167)
		::coopy::CellInfo cell = ::coopy::CellInfo_obj::__new();		HX_STACK_VAR(cell,"cell");
		HX_STACK_LINE(168)
		::String corner = tt->getCellText((int)0,(int)0);		HX_STACK_VAR(corner,"corner");
		HX_STACK_LINE(169)
		int off;		HX_STACK_VAR(off,"off");
		HX_STACK_LINE(169)
		if (((corner == HX_CSTRING("@:@")))){
			HX_STACK_LINE(169)
			off = (int)1;
		}
		else{
			HX_STACK_LINE(169)
			off = (int)0;
		}
		HX_STACK_LINE(170)
		if (((off > (int)0))){
			HX_STACK_LINE(171)
			if (((bool((rows->__Field(HX_CSTRING("get_width"),true)() <= (int)1)) || bool((rows->__Field(HX_CSTRING("get_height"),true)() <= (int)1))))){
				HX_STACK_LINE(171)
				return null();
			}
		}
		HX_STACK_LINE(173)
		{
			HX_STACK_LINE(173)
			int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(173)
			int _g = rows->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(173)
			while(((_g1 < _g))){
				HX_STACK_LINE(173)
				int row = (_g1)++;		HX_STACK_VAR(row,"row");
				HX_STACK_LINE(175)
				bool open = false;		HX_STACK_VAR(open,"open");
				HX_STACK_LINE(177)
				::String txt = tt->getCellText(off,row);		HX_STACK_VAR(txt,"txt");
				HX_STACK_LINE(178)
				if (((txt == null()))){
					HX_STACK_LINE(178)
					txt = HX_CSTRING("");
				}
				HX_STACK_LINE(179)
				::coopy::DiffRender_obj::examineCell((int)0,row,txt,HX_CSTRING(""),txt,corner,cell);
				HX_STACK_LINE(180)
				::String row_mode = cell->category;		HX_STACK_VAR(row_mode,"row_mode");
				HX_STACK_LINE(181)
				if (((row_mode == HX_CSTRING("spec")))){
					HX_STACK_LINE(182)
					change_row = row;
				}
				HX_STACK_LINE(185)
				render->beginRow(row_mode);
				HX_STACK_LINE(187)
				{
					HX_STACK_LINE(187)
					int _g3 = (int)0;		HX_STACK_VAR(_g3,"_g3");
					HX_STACK_LINE(187)
					int _g2 = rows->__Field(HX_CSTRING("get_width"),true)();		HX_STACK_VAR(_g2,"_g2");
					HX_STACK_LINE(187)
					while(((_g3 < _g2))){
						HX_STACK_LINE(187)
						int c = (_g3)++;		HX_STACK_VAR(c,"c");
						HX_STACK_LINE(188)
						::coopy::DiffRender_obj::examineCell(c,row,tt->getCellText(c,row),(  (((change_row >= (int)0))) ? ::String(tt->getCellText(c,change_row)) : ::String(HX_CSTRING("")) ),txt,corner,cell);
						HX_STACK_LINE(195)
						render->insertCell((  ((this->pretty_arrows)) ? ::String(cell->pretty_value) : ::String(cell->value) ),cell->category_given_tr);
					}
				}
				HX_STACK_LINE(198)
				render->endRow();
			}
		}
		HX_STACK_LINE(200)
		render->endTable();
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC1(DiffRender_obj,render,(void))

::String DiffRender_obj::sampleCss( ){
	HX_STACK_FRAME("coopy.DiffRender","sampleCss",0xf7e1db88,"coopy.DiffRender.sampleCss","coopy/DiffRender.hx",204,0xe6f800c0)
	HX_STACK_THIS(this)
	HX_STACK_LINE(204)
	return HX_CSTRING(".highlighter .add { \n  background-color: #7fff7f;\n}\n\n.highlighter .remove { \n  background-color: #ff7f7f;\n}\n\n.highlighter td.modify { \n  background-color: #7f7fff;\n}\n\n.highlighter td.conflict { \n  background-color: #f00;\n}\n\n.highlighter .spec { \n  background-color: #aaa;\n}\n\n.highlighter .move { \n  background-color: #ffa;\n}\n\n.highlighter .null { \n  color: #888;\n}\n\n.highlighter table { \n  border-collapse:collapse;\n}\n\n.highlighter td, .highlighter th {\n  border: 1px solid #2D4068;\n  padding: 3px 7px 2px;\n}\n\n.highlighter th, .highlighter .header { \n  background-color: #aaf;\n  font-weight: bold;\n  padding-bottom: 4px;\n  padding-top: 5px;\n  text-align:left;\n}\n\n.highlighter tr:first-child td {\n  border-top: 1px solid #2D4068;\n}\n\n.highlighter td:first-child { \n  border-left: 1px solid #2D4068;\n}\n\n.highlighter td {\n  empty-cells: show;\n}\n");
}


HX_DEFINE_DYNAMIC_FUNC0(DiffRender_obj,sampleCss,return )

Void DiffRender_obj::completeHtml( ){
{
		HX_STACK_FRAME("coopy.DiffRender","completeHtml",0xe25dd455,"coopy.DiffRender.completeHtml","coopy/DiffRender.hx",263,0xe6f800c0)
		HX_STACK_THIS(this)
		HX_STACK_LINE(264)
		this->text_to_insert->insert((int)0,HX_CSTRING("<html>\n<meta charset='utf-8'>\n<head>\n<style TYPE='text/css'>\n"));
		HX_STACK_LINE(269)
		this->text_to_insert->insert((int)1,this->sampleCss());
		HX_STACK_LINE(270)
		this->text_to_insert->insert((int)2,HX_CSTRING("</style>\n</head>\n<body>\n<div class='highlighter'>\n"));
		HX_STACK_LINE(275)
		this->text_to_insert->push(HX_CSTRING("</div>\n</body>\n</html>\n"));
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(DiffRender_obj,completeHtml,(void))

Void DiffRender_obj::examineCell( int x,int y,::String value,::String vcol,::String vrow,::String vcorner,::coopy::CellInfo cell){
{
		HX_STACK_FRAME("coopy.DiffRender","examineCell",0x54a587f2,"coopy.DiffRender.examineCell","coopy/DiffRender.hx",81,0xe6f800c0)
		HX_STACK_ARG(x,"x")
		HX_STACK_ARG(y,"y")
		HX_STACK_ARG(value,"value")
		HX_STACK_ARG(vcol,"vcol")
		HX_STACK_ARG(vrow,"vrow")
		HX_STACK_ARG(vcorner,"vcorner")
		HX_STACK_ARG(cell,"cell")
		HX_STACK_LINE(82)
		cell->category = HX_CSTRING("");
		HX_STACK_LINE(83)
		cell->category_given_tr = HX_CSTRING("");
		HX_STACK_LINE(84)
		cell->separator = HX_CSTRING("");
		HX_STACK_LINE(85)
		cell->conflicted = false;
		HX_STACK_LINE(86)
		cell->updated = false;
		HX_STACK_LINE(87)
		cell->pvalue = cell->lvalue = cell->rvalue = null();
		HX_STACK_LINE(88)
		cell->value = value;
		HX_STACK_LINE(89)
		if (((cell->value == null()))){
			HX_STACK_LINE(89)
			cell->value = HX_CSTRING("");
		}
		HX_STACK_LINE(90)
		cell->pretty_value = cell->value;
		HX_STACK_LINE(91)
		if (((vrow == null()))){
			HX_STACK_LINE(91)
			vrow = HX_CSTRING("");
		}
		HX_STACK_LINE(92)
		if (((vcol == null()))){
			HX_STACK_LINE(92)
			vcol = HX_CSTRING("");
		}
		HX_STACK_LINE(93)
		bool removed_column = false;		HX_STACK_VAR(removed_column,"removed_column");
		HX_STACK_LINE(94)
		if (((vrow == HX_CSTRING(":")))){
			HX_STACK_LINE(95)
			cell->category = HX_CSTRING("move");
		}
		HX_STACK_LINE(97)
		if (((vcol.indexOf(HX_CSTRING("+++"),null()) >= (int)0))){
			HX_STACK_LINE(98)
			cell->category_given_tr = cell->category = HX_CSTRING("add");
		}
		else{
			HX_STACK_LINE(99)
			if (((vcol.indexOf(HX_CSTRING("---"),null()) >= (int)0))){
				HX_STACK_LINE(100)
				cell->category_given_tr = cell->category = HX_CSTRING("remove");
				HX_STACK_LINE(101)
				removed_column = true;
			}
		}
		HX_STACK_LINE(103)
		if (((vrow == HX_CSTRING("!")))){
			HX_STACK_LINE(104)
			cell->category = HX_CSTRING("spec");
		}
		else{
			HX_STACK_LINE(105)
			if (((vrow == HX_CSTRING("@@")))){
				HX_STACK_LINE(106)
				cell->category = HX_CSTRING("header");
			}
			else{
				HX_STACK_LINE(107)
				if (((vrow == HX_CSTRING("+++")))){
					HX_STACK_LINE(108)
					if ((!(removed_column))){
						HX_STACK_LINE(109)
						cell->category = HX_CSTRING("add");
					}
				}
				else{
					HX_STACK_LINE(111)
					if (((vrow == HX_CSTRING("---")))){
						HX_STACK_LINE(112)
						cell->category = HX_CSTRING("remove");
					}
					else{
						HX_STACK_LINE(113)
						if (((vrow.indexOf(HX_CSTRING("->"),null()) >= (int)0))){
							HX_STACK_LINE(114)
							if ((!(removed_column))){
								HX_STACK_LINE(115)
								Array< ::String > tokens = vrow.split(HX_CSTRING("!"));		HX_STACK_VAR(tokens,"tokens");
								HX_STACK_LINE(116)
								::String full = vrow;		HX_STACK_VAR(full,"full");
								HX_STACK_LINE(117)
								::String part = tokens->__get((int)1);		HX_STACK_VAR(part,"part");
								HX_STACK_LINE(118)
								if (((part == null()))){
									HX_STACK_LINE(118)
									part = full;
								}
								HX_STACK_LINE(119)
								if (((cell->value.indexOf(part,null()) >= (int)0))){
									HX_STACK_LINE(120)
									::String cat = HX_CSTRING("modify");		HX_STACK_VAR(cat,"cat");
									HX_STACK_LINE(121)
									::String div = part;		HX_STACK_VAR(div,"div");
									HX_STACK_LINE(123)
									if (((part != full))){
										HX_STACK_LINE(124)
										if (((cell->value.indexOf(full,null()) >= (int)0))){
											HX_STACK_LINE(125)
											div = full;
											HX_STACK_LINE(126)
											cat = HX_CSTRING("conflict");
											HX_STACK_LINE(127)
											cell->conflicted = true;
										}
									}
									HX_STACK_LINE(130)
									cell->updated = true;
									HX_STACK_LINE(131)
									cell->separator = div;
									HX_STACK_LINE(132)
									tokens = cell->pretty_value.split(div);
									HX_STACK_LINE(133)
									cell->pretty_value = tokens->join(::String::fromCharCode((int)8594));
									HX_STACK_LINE(134)
									cell->category_given_tr = cell->category = cat;
									HX_STACK_LINE(135)
									int offset;		HX_STACK_VAR(offset,"offset");
									HX_STACK_LINE(135)
									if ((cell->conflicted)){
										HX_STACK_LINE(135)
										offset = (int)1;
									}
									else{
										HX_STACK_LINE(135)
										offset = (int)0;
									}
									HX_STACK_LINE(136)
									cell->lvalue = tokens->__get(offset);
									HX_STACK_LINE(137)
									cell->rvalue = tokens->__get((offset + (int)1));
									HX_STACK_LINE(138)
									if ((cell->conflicted)){
										HX_STACK_LINE(138)
										cell->pvalue = tokens->__get((int)0);
									}
								}
							}
						}
					}
				}
			}
		}
	}
return null();
}


STATIC_HX_DEFINE_DYNAMIC_FUNC7(DiffRender_obj,examineCell,(void))

::coopy::CellInfo DiffRender_obj::renderCell( ::coopy::TableText tt,int x,int y){
	HX_STACK_FRAME("coopy.DiffRender","renderCell",0x6f15a769,"coopy.DiffRender.renderCell","coopy/DiffRender.hx",146,0xe6f800c0)
	HX_STACK_ARG(tt,"tt")
	HX_STACK_ARG(x,"x")
	HX_STACK_ARG(y,"y")
	HX_STACK_LINE(147)
	::coopy::CellInfo cell = ::coopy::CellInfo_obj::__new();		HX_STACK_VAR(cell,"cell");
	HX_STACK_LINE(148)
	::String corner = tt->getCellText((int)0,(int)0);		HX_STACK_VAR(corner,"corner");
	HX_STACK_LINE(149)
	int off;		HX_STACK_VAR(off,"off");
	HX_STACK_LINE(149)
	if (((corner == HX_CSTRING("@:@")))){
		HX_STACK_LINE(149)
		off = (int)1;
	}
	else{
		HX_STACK_LINE(149)
		off = (int)0;
	}
	HX_STACK_LINE(153)
	::String _g15 = tt->getCellText(x,y);		HX_STACK_VAR(_g15,"_g15");
	HX_STACK_LINE(154)
	::String _g16 = tt->getCellText(x,off);		HX_STACK_VAR(_g16,"_g16");
	HX_STACK_LINE(151)
	::coopy::DiffRender_obj::examineCell(x,y,_g15,_g16,tt->getCellText(off,y),corner,cell);
	HX_STACK_LINE(158)
	return cell;
}


STATIC_HX_DEFINE_DYNAMIC_FUNC3(DiffRender_obj,renderCell,return )


DiffRender_obj::DiffRender_obj()
{
}

void DiffRender_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(DiffRender);
	HX_MARK_MEMBER_NAME(text_to_insert,"text_to_insert");
	HX_MARK_MEMBER_NAME(td_open,"td_open");
	HX_MARK_MEMBER_NAME(td_close,"td_close");
	HX_MARK_MEMBER_NAME(open,"open");
	HX_MARK_MEMBER_NAME(pretty_arrows,"pretty_arrows");
	HX_MARK_END_CLASS();
}

void DiffRender_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(text_to_insert,"text_to_insert");
	HX_VISIT_MEMBER_NAME(td_open,"td_open");
	HX_VISIT_MEMBER_NAME(td_close,"td_close");
	HX_VISIT_MEMBER_NAME(open,"open");
	HX_VISIT_MEMBER_NAME(pretty_arrows,"pretty_arrows");
}

Dynamic DiffRender_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 4:
		if (HX_FIELD_EQ(inName,"open") ) { return open; }
		if (HX_FIELD_EQ(inName,"html") ) { return html_dyn(); }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"insert") ) { return insert_dyn(); }
		if (HX_FIELD_EQ(inName,"endRow") ) { return endRow_dyn(); }
		if (HX_FIELD_EQ(inName,"render") ) { return render_dyn(); }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"td_open") ) { return td_open; }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"td_close") ) { return td_close; }
		if (HX_FIELD_EQ(inName,"beginRow") ) { return beginRow_dyn(); }
		if (HX_FIELD_EQ(inName,"endTable") ) { return endTable_dyn(); }
		if (HX_FIELD_EQ(inName,"toString") ) { return toString_dyn(); }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"sampleCss") ) { return sampleCss_dyn(); }
		break;
	case 10:
		if (HX_FIELD_EQ(inName,"renderCell") ) { return renderCell_dyn(); }
		if (HX_FIELD_EQ(inName,"beginTable") ) { return beginTable_dyn(); }
		if (HX_FIELD_EQ(inName,"insertCell") ) { return insertCell_dyn(); }
		break;
	case 11:
		if (HX_FIELD_EQ(inName,"examineCell") ) { return examineCell_dyn(); }
		break;
	case 12:
		if (HX_FIELD_EQ(inName,"completeHtml") ) { return completeHtml_dyn(); }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"pretty_arrows") ) { return pretty_arrows; }
		break;
	case 14:
		if (HX_FIELD_EQ(inName,"text_to_insert") ) { return text_to_insert; }
		break;
	case 15:
		if (HX_FIELD_EQ(inName,"usePrettyArrows") ) { return usePrettyArrows_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic DiffRender_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 4:
		if (HX_FIELD_EQ(inName,"open") ) { open=inValue.Cast< bool >(); return inValue; }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"td_open") ) { td_open=inValue.Cast< ::String >(); return inValue; }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"td_close") ) { td_close=inValue.Cast< ::String >(); return inValue; }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"pretty_arrows") ) { pretty_arrows=inValue.Cast< bool >(); return inValue; }
		break;
	case 14:
		if (HX_FIELD_EQ(inName,"text_to_insert") ) { text_to_insert=inValue.Cast< Array< ::String > >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void DiffRender_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("text_to_insert"));
	outFields->push(HX_CSTRING("td_open"));
	outFields->push(HX_CSTRING("td_close"));
	outFields->push(HX_CSTRING("open"));
	outFields->push(HX_CSTRING("pretty_arrows"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	HX_CSTRING("examineCell"),
	HX_CSTRING("renderCell"),
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(DiffRender_obj,text_to_insert),HX_CSTRING("text_to_insert")},
	{hx::fsString,(int)offsetof(DiffRender_obj,td_open),HX_CSTRING("td_open")},
	{hx::fsString,(int)offsetof(DiffRender_obj,td_close),HX_CSTRING("td_close")},
	{hx::fsObject,(int)offsetof(DiffRender_obj,open),HX_CSTRING("open")},
	{hx::fsObject,(int)offsetof(DiffRender_obj,pretty_arrows),HX_CSTRING("pretty_arrows")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("text_to_insert"),
	HX_CSTRING("td_open"),
	HX_CSTRING("td_close"),
	HX_CSTRING("open"),
	HX_CSTRING("pretty_arrows"),
	HX_CSTRING("usePrettyArrows"),
	HX_CSTRING("insert"),
	HX_CSTRING("beginTable"),
	HX_CSTRING("beginRow"),
	HX_CSTRING("insertCell"),
	HX_CSTRING("endRow"),
	HX_CSTRING("endTable"),
	HX_CSTRING("html"),
	HX_CSTRING("toString"),
	HX_CSTRING("render"),
	HX_CSTRING("sampleCss"),
	HX_CSTRING("completeHtml"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(DiffRender_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(DiffRender_obj::__mClass,"__mClass");
};

#endif

Class DiffRender_obj::__mClass;

void DiffRender_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.DiffRender"), hx::TCanCast< DiffRender_obj> ,sStaticFields,sMemberFields,
	&__CreateEmpty, &__Create,
	&super::__SGetClass(), 0, sMarkStatics
#ifdef HXCPP_VISIT_ALLOCS
    , sVisitStatics
#endif
#ifdef HXCPP_SCRIPTABLE
    , sMemberStorageInfo
#endif
);
}

void DiffRender_obj::__boot()
{
}

} // end namespace coopy
