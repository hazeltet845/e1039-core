/// OnlMonV1495.C
#include <iomanip>
#include <TH1D.h>
#include <interface_main/SQRun.h>
#include <interface_main/SQStringMap.h>
#include <interface_main/SQScaler.h>
#include <interface_main/SQSlowCont.h>
#include <interface_main/SQEvent.h>
#include <interface_main/SQHitVector.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllHistoManager.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>
#include <geom_svc/GeomSvc.h>
//#include <chan_map/CalibParamInTimeV1495.h>
#include "OnlMonServer.h"
#include "OnlMonV1495.h"
#include "UtilHist.h"
using namespace std;

OnlMonV1495::OnlMonV1495(const HodoType_t type, const int lvl) : m_type(type), m_lvl(lvl)
{
  NumCanvases(2);
  m_n_pl = 2;
  switch (m_type) {
  case H1X:  m_pl0 = 31;  Name("OnlMonV1495H1X" );  Title("H1X v1495" );  break;
  case H2X:  m_pl0 = 37;  Name("OnlMonV1495H2X" );  Title("H2X v1495" );  break;
  case H3X:  m_pl0 = 39;  Name("OnlMonV1495H3X" );  Title("H3X v1495" );  break;
  case H4X:  m_pl0 = 45;  Name("OnlMonV1495H4X" );  Title("H4X v1495" );  break;
  case H1Y:  m_pl0 = 33;  Name("OnlMonV1495H1Y" );  Title("H1Y v1495" );  break;
  case H2Y:  m_pl0 = 35;  Name("OnlMonV1495H2Y" );  Title("H2Y v1495" );  break;
  case H4Y1: m_pl0 = 41;  Name("OnlMonV1495H4Y1");  Title("H4Y1 v1495");  break;
  case H4Y2: m_pl0 = 43;  Name("OnlMonV1495H4Y2");  Title("H4Y2 v1495");  break;
  }
}

int OnlMonV1495::InitOnlMon(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int OnlMonV1495::InitRunOnlMon(PHCompositeNode* topNode)
{
  SQRun* run_header = findNode::getClass<SQRun>(topNode, "SQRun");
  if (!run_header) return Fun4AllReturnCodes::ABORTEVENT;

  GeomSvc* geom = GeomSvc::instance();
  //CalibParamInTimeV1495 calib;
  //calib.SetMapIDbyDB(run_header->get_run_id());
  //calib.ReadFromDB();

  Fun4AllHistoManager* hm = new Fun4AllHistoManager(Name());
  OnlMonServer::instance()->registerHistoManager(hm);

  ostringstream oss;
  for (int pl = 0; pl < m_n_pl; pl++) {
    string name = geom->getDetectorName(m_pl0 + pl);
    int n_ele = geom->getPlaneNElements(m_pl0 + pl); 
    oss.str("");
    oss << "h1_ele_" << pl;
    h1_ele[pl] = new TH1D(oss.str().c_str(), "", n_ele, 0.5, n_ele+0.5);
    oss.str("");
    oss << name << ";Element ID;Hit count";
    h1_ele[pl]->SetTitle(oss.str().c_str());

    const double DT = 1.0; // 1 ns per single count of v1495 TDC
    const int NT = 2000;
    const double T0 = 0.5*DT;
    const double T1 = (NT+0.5)*DT;

    //double center, width;
    //calib.Find(m_pl0 + pl, 1, m_lvl, center, width);
    oss.str("");
    oss << "h1_time_" << pl;
    h1_time[pl] = new TH1D(oss.str().c_str(), "", NT, T0, T1);
    //h1_time[pl] = new TH1D(oss.str().c_str(), "", 100, center-2.5*width, center+2.5*width);

    oss.str("");
    oss << name << ";tdcTime;Hit count";
    h1_time[pl]->SetTitle(oss.str().c_str());

    oss.str("");
    oss << "h1_time_in_" << pl;
    h1_time_in[pl] = new TH1D(oss.str().c_str(), "", NT, T0, T1);
    oss.str("");
    oss << name << ";tdcTime;In-time hit count";
    h1_time_in[pl]->SetTitle(oss.str().c_str());

    hm->registerHisto(h1_ele    [pl]);
    hm->registerHisto(h1_time   [pl]);
    hm->registerHisto(h1_time_in[pl]);
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

int OnlMonV1495::ProcessEventOnlMon(PHCompositeNode* topNode)
{
  SQEvent*     event_header = findNode::getClass<SQEvent    >(topNode, "SQEvent");
  SQHitVector*      hit_vec = findNode::getClass<SQHitVector>(topNode, "SQTriggerHitVector");
  if (!event_header || !hit_vec) return Fun4AllReturnCodes::ABORTEVENT;

  for (SQHitVector::ConstIter it = hit_vec->begin(); it != hit_vec->end(); it++) {
    int pl = (*it)->get_detector_id() - m_pl0;
    if (pl < 0 || pl >= m_n_pl || (*it)->get_level() != m_lvl) continue;
    h1_ele [pl]->Fill((*it)->get_element_id());
    h1_time[pl]->Fill((*it)->get_tdc_time  ());
    if ((*it)->is_in_time()) h1_time_in[pl]->Fill((*it)->get_tdc_time());
  }
  
  return Fun4AllReturnCodes::EVENT_OK;
}

int OnlMonV1495::EndOnlMon(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int OnlMonV1495::FindAllMonHist()
{
  ostringstream oss;
  for (int pl = 0; pl < m_n_pl; pl++) {
    oss.str("");
    oss << "h1_ele_" << pl;
    h1_ele[pl] = (TH1*)FindMonObj(oss.str().c_str());
    if (! h1_ele[pl]) return 1;
    oss.str("");
    oss << "h1_time_" << pl;
    h1_time[pl] = (TH1*)FindMonObj(oss.str().c_str());
    if (! h1_time[pl]) return 1;
    oss.str("");
    oss << "h1_time_in_" << pl;
    h1_time_in[pl] = (TH1*)FindMonObj(oss.str().c_str());
    if (! h1_time_in[pl]) return 1;
  }
  return 0;
}

int OnlMonV1495::DrawMonitor()
{
  OnlMonCanvas* can0 = GetCanvas(0);
  TPad* pad0 = can0->GetMainPad();
  pad0->SetGrid();
  pad0->Divide(1, 2);
  for (int pl = 0; pl < m_n_pl; pl++) {
    pad0->cd(pl+1);
    //if (h1_ele[pl]->Integral() > 1000) gPad->SetLogy();
    h1_ele[pl]->Draw();
  }
  can0->AddMessage("Always Okay ;^D");
  can0->SetStatus(OnlMonCanvas::OK);

  OnlMonCanvas* can1 = GetCanvas(1);
  TPad* pad1 = can1->GetMainPad();
  pad1->SetGrid();
  pad1->Divide(1, 2);
  for (int pl = 0; pl < m_n_pl; pl++) {
    pad1->cd(pl+1);
    //if (h1_time[pl]->Integral() > 1000) gPad->SetLogy();
    UtilHist::AutoSetRange(h1_time[pl]);
    h1_time[pl]->Draw();
    h1_time_in[pl]->SetLineColor(kRed);
    h1_time_in[pl]->SetFillColor(kRed-7);
    h1_time_in[pl]->Draw("same");
  }
  can1->AddMessage("Always Okay ;^D");
  can1->SetStatus(OnlMonCanvas::OK);

  return 0;
}
