// #include "DrawFrame.hpp"
// #include <iostream>
// #include <cstdlib>
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

// DrawFrame::DrawFrame(const wxString &title, const wxPoint &pos, const wxSize &size, ParserXML *_parser)
//     : wxFrame(NULL, wxID_ANY, title, pos, size)
// {
//     // parser = _parser;
//     // // create the panel
//     // m_panel = new wxPanel(this);
//     // // create the button
//     // m_button = new wxButton(m_panel, wxID_ANY, "Draw Line");
//     // // connect the button click event to the custom event handler function
//     // m_button->Bind(wxEVT_BUTTON, &DrawFrame::OnButtonClick, this);
//     // // connect the paint event to the custom event handler function
//     // m_panel->Bind(wxEVT_PAINT, &DrawFrame::OnPaint, this);

//     // wxFlexGridSizer* gridSizer = new wxFlexGridSizer(101, 101, 0, 0);
//     // gridSizer->AddGrowableCol(0, 1);
//     // gridSizer->AddGrowableRow(0, 1);
//     // for (int i = 1; i < 101; i++) {
//     //     gridSizer->AddGrowableCol(i, 10);
//     //     gridSizer->AddGrowableRow(i, 10);
//     // }
//     // wxStaticBoxSizer* borderSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Grid");
//     // borderSizer->Add(gridSizer, 1, wxEXPAND);
//     // SetSizer(borderSizer);

//     // for (int i = 0; i < 100; i++) {
//     //     for (int j = 0; j < 100; j++) {
//     //         wxPanel* cell = new wxPanel(this);
//     //         cell->SetMinSize(wxSize(10, 10));
//     //         gridSizer->Add(cell, 1, wxEXPAND);
//     //     }
//     // }
// }

// void DrawFrame::OnButtonClick(wxCommandEvent &event)
// {
//     // refresh the panel to trigger the paint event

//     // write vectors to a JSON file
//     // try
//     // {
//     //     json j;
//     //     j["vectors"] = vectors;
//     //     std::ofstream out(".\\vectors.json");
//     //     out << j.dump(4);
//     //     out.close();

//     //     // call the Python script
//     //     system("python script.py vectors.json");
//     // }
//     // catch (const std::exception &e)
//     // {
//     //     std::cerr << e.what();
//     // }
// }

// void DrawFrame::drawObjects(wxPaintDC &dc, int &x, int &y, int &number, std::vector<Linear> &linears, std::vector<Point> &points)
// {
//     // auto point_it = std::find_if(points.begin(), points.end(),
//     //                             [number](Point& p) {
//     //                                 return p.getSectionId() == number;
//     //                             });
//     // auto linear_it = std::find_if(linears.begin(), linears.end(),
//     //                              [number](Linear& l) {
//     //                                 return l.getSectionId() == number;
//     //                              });
//     // if (point_it != points.end())
//     // {
//     // Point& next_point = *point_it;
//     // dc.DrawRectangle(x, y - 20, 40, 40);
//     // x += 40;
//     // dc.DrawLine(x, y, x + 40, y);
//     // dc.DrawLine(x-20,y+20,x-20,y+60);
//     // int minus = next_point.getMinus();
//     // int plus = next_point.getPlus();
//     // int stem = next_point.getStem();
//     // if( number == minus){
//     //         auto point_1 = std::find_if(points.begin(), points.end(),
//     //                         [plus](Point& p) {
//     //                             return p.getSectionId() == plus;
//     //                         });
//     //         auto linear_1 = std::find_if(linears.begin(), linears.end(),
//     //                          [plus](Linear& l) {
//     //                             return l.getSectionId() == plus;
//     //                          });
//     //         auto point_2 = std::find_if(points.begin(), points.end(),
//     //                         [stem](Point& p) {
//     //                             return p.getSectionId() == stem;
//     //                         });
//     //         auto linear_2 = std::find_if(linears.begin(), linears.end(),
//     //                          [stem](Linear& l) {
//     //                             return l.getSectionId() == stem;
//     //                          });
//     //         if(linear_1 != linears.end() && linear_2 != linears.end()){
//     //             // Linear& next_linear1 = *linear_1;
//     //             // int number1 = next_linear1.getDownNeig();
//     //             // dc.DrawLine(x, y, x + 40, y);
//     //             // x += 40;
//     //             // drawObjects(dc, x, y, number1, linears, points);
//     //             // Linear& next_linear2 = *linear_2;
//     //             // int number2 = next_linear2.getDownNeig();
//     //             // dc.DrawLine(x, y, x + 40, y);
//     //             // x += 40;
//     //             // drawObjects(dc, x, y, number2, linears, points);
//     //         }
//     //         else if(linear_1 != linears.end() && point_2 != points.end()){

//     //         }
//     //         else if(point_1 != points.end() && linear_2 != linears.end()){

//     //         }
//     //         else if(point_1 != points.end() && point_2 != points.end()){

//     //         }
//     // else if ( number == plus)
//     // {
//     //         auto point_1 = std::find_if(points.begin(), points.end(),
//     //                         [minus](Point& p) {
//     //                             return p.getSectionId() == minus;
//     //                         });
//     //         auto linear_1 = std::find_if(linears.begin(), linears.end(),
//     //                          [minus](Linear& l) {
//     //                             return l.getSectionId() == minus;
//     //                          });
//     //         auto point_2 = std::find_if(points.begin(), points.end(),
//     //                         [stem](Point& p) {
//     //                             return p.getSectionId() == stem;
//     //                         });
//     //         auto linear_2 = std::find_if(linears.begin(), linears.end(),
//     //                          [stem](Linear& l) {
//     //                             return l.getSectionId() == stem;
//     //                          });
//     // }
//     // else if ( number == stem ){
//     //         auto point_1 = std::find_if(points.begin(), points.end(),
//     //                         [plus](Point& p) {
//     //                             return p.getSectionId() == plus;
//     //                         });
//     //         auto linear_1 = std::find_if(linears.begin(), linears.end(),
//     //                          [plus](Linear& l) {
//     //                             return l.getSectionId() == plus;
//     //                          });
//     //         auto point_2 = std::find_if(points.begin(), points.end(),
//     //                         [minus](Point& p) {
//     //                             return p.getSectionId() == minus;
//     //                         });
//     //         auto linear_2 = std::find_if(linears.begin(), linears.end(),
//     //                          [minus](Linear& l) {
//     //                             return l.getSectionId() == minus;
//     //                          });
//     // if(linear_1 != linears.end() && linear_2 != linears.end()){
//     //     Linear& next_linear1 = *linear_1;
//     //     int number1 = next_linear1.getDownNeig();
//     //     dc.DrawLine(x, y, x + 40, y);
//     //     x += 40;
//     //     drawObjects(dc, x, y, number1, linears, points);
//     //     Linear& next_linear2 = *linear_2;
//     //     int number2 = next_linear2.getDownNeig();
//     //     dc.DrawLine(x, y, x + 40, y);
//     //     x += 40;
//     //     drawObjects(dc, x, y, number2, linears, points);
//     // }
//     // else if(linear_1 != linears.end() && point_2 != points.end()){

//     // }
//     // else if(point_1 != points.end() && linear_2 != linears.end()){

//     // }
//     // else if(point_1 != points.end() && point_2 != points.end()){

//     // }
//     // drawObjects(dc,x,y,)

//     // }
//     // else if (linear_it != linears.end())
//     // {
//     // Linear &next_linear = *linear_it;
//     // // int number = next_linear.getDownNeig();
//     // dc.DrawLine(x, y, x + 40, y);
 
//     // }
// }

// void DrawFrame::OnPaint(wxPaintEvent &event)
// {
//     // wxPaintDC dc(m_panel);
//     // wxSize size = m_panel->GetSize();
//     // int centerX = (size.GetWidth() / 2) - 300;
//     // int centerY = (size.GetHeight() / 2);
//     // int x = centerX;
//     // int y = centerY;
//     // std::vector<Linear> linears = parser->getNl().getLinears();
//     // std::vector<Point> points = parser->getNl().getPoints();

//     // auto it = std::find_if(linears.begin(), linears.end(),
//     //                        [](Linear &l)
//     //                        {
//     //                            return (l.getUpNeig() == -1);
//     //                        });
//     // if (it != linears.end())
//     // {
//     //     Linear &next_linear = *it;
//     //     int number = next_linear.getDownNeig();
//     //     dc.DrawLine(x, y, x + 40, y);
//     //     x += 40;
//     //     // drawObjects(dc, x, y, number, linears, points);
//     // }
//     // if (it != linears.end()) {
//     //     // The Linear object with upNeig equal to -1 has been found
//     //     Linear& init_linear = *it;

//     //     while(!linears.empty() || !points.em){

//     //     }
//     //     dc.DrawCircle(x, y, 50);

//     //     // Do something with found_linear
//     // }

//     //  DOWN = LEFT RIGHT
//     //  UP = RIGHT LEFT
//     // Do something with found_linear

//     // if (merge.size() == parser->getPlCorrispondence().size())

//     // for (int i = 0; i < (int)parser->getPlCorrispondence().size(); i++) {
//     //     if(parser->getNl().at(i).getUpNeig() == -1){
//     //         dc.DrawLine(x, y, x + 40, y);
//     //         x += 40;
//     //     }
//     //     else {
//     //         dc.DrawRectangle(x, y - 20 , 40, 40);
//     //         x += 40;
//     //     }
//     // }
// }
