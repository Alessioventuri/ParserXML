#pragma once
#include "writer.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

/*
UMC model creation for railway interlocking routes
*/

class writerUMC : public writer{
private :
    // ONE TRAIN
    string defaultUMCsetupOneRoute(unique_ptr<ParserXML>&,int);

    string pointObjectUmcOneRoute (Route, map<int,string>, NetworkLayout);
    string linearObjectUmcOneRoute(Route, int, map<int,string>, map<int,string>, NetworkLayout);
    string signalObjectUmcOneRoute(Route, map<int,string>, map<int,string>, NetworkLayout);
    string trainObjectUmcOneRoute (Route, int, map<int,string>);

    string findMb                 (Route, NetworkLayout, int, map<int,string>);

    string abstractionUmcOneRoute (Route, int, map<int,string>, map<int,string>, NetworkLayout);
    string derailAbsOneRoute      (Route, int, map<int,string>, NetworkLayout);
    string brokenSignalsOneRoute  (Route, int, map<int,string>, map<int,string>, NetworkLayout);
    string trainArrived           (Route, int, map<int,string>) const;

    // TWO TRAINs
    string defaultUMCsetupTwoRoute(unique_ptr<ParserXML>&, int, int);
    string stringCombinerNl       (unique_ptr<ParserXML>&, int, int) const;
    string stringCombinerIl       (unique_ptr<ParserXML>&, int, int) const;
    string stringCombinerId       (unique_ptr<ParserXML>&, int, int) const;
    string abstractionUmcTwoRoute (const Route&,const Route&, int, int, map<int,string>, map<int,string>, NetworkLayout );
    string pointObjectUmcTwoRoute (Route &, Route &, map<int,string> , NetworkLayout ) const;


public:
    writerUMC() = default;
    ~writerUMC() = default;
    void writeFile                (string,unique_ptr<ParserXML>&,int,int,int,int) override;

    friend class writer;

    string codeUMC = R"(
    Class Train is

    Signals:
        go, no_go;

    Vars:
        id_itinerary: int;
        route_nodes: obj[];
        position: int := 0;
        node: obj;

    State Top = READY, WAIT_OK, MOVE, ARRIVED, STOP

    Transitions:
        READY    -> WAIT_OK { - /
                        a:obj:=route_nodes[position];a.req(self, id_itinerary);
                    }

        WAIT_OK -> READY    { no_go }

        WAIT_OK -> MOVE { go }

        MOVE -> MOVE { - [position < route_nodes.length-1] /
                                    pos: int := position;
                                    position := -1;
                                    node := null;
                                    b:obj:=route_nodes[pos];b.sensorOff(self, id_itinerary);
                                    c:obj:=route_nodes[pos+1];c.sensorOn(self, id_itinerary);
                                    position := pos+1;
                                    node := route_nodes[position];
        }

        MOVE -> ARRIVED  { - [position = route_nodes.length-1] }

    end Train

    Class Linear is

    Signals:
        req(sender: obj, id_itinerary: int);
        ack(sender: obj, id_itinerary: int);
        nack(sender: obj, id_itinerary: int);
        commit(sender: obj, id_itinerary: int);
        agree(sender: obj, id_itinerary: int);
        disagree(sender: obj, id_itinerary: int);
        green(id_itinerary:int);
        red(id_itinerary:int);

    Operations:
        sensorOn(sender: obj, id_itinerary);
        sensorOff(sender: obj, id_itinerary);

    Vars:
        next: obj[];
        prev: obj[];
        sign : obj[];
        train: obj := null;

    State Top = NOT_RESERVED, WAIT_ACK, WAIT_COMMIT, WAIT_AGREE, CHECK_LIGHT_START, RESERVED, TRAIN_IN_TRANSIT, CHECK_LIGHT_END
    State CHECK_LIGHT_END Defers  req( sender: obj, id_itinerary: int), red(id_itinerary: int), nack(sender : obj, id_itinerary : int)

    Transitions

        NOT_RESERVED -> WAIT_ACK { req(sender, id_itinerary)
                        [(train=null or sender=train) and next[id_itinerary] /= null]   /
                            d:obj:=next[id_itinerary];d.req(self, id_itinerary);}

        NOT_RESERVED -> WAIT_COMMIT { req(sender, id_itinerary)
                        [(train=null or sender=train) and next[id_itinerary] = null] /
                        e:obj:=prev[id_itinerary];e.ack(self, id_itinerary); }

        WAIT_ACK    -> WAIT_COMMIT { ack(sender, id_itinerary) [prev[id_itinerary] /= null] /
                        f:obj:=prev[id_itinerary];f.ack(self, id_itinerary); }

        WAIT_ACK    -> WAIT_AGREE  { ack(sender, id_itinerary) [prev[id_itinerary] = null] /
                        g:obj:=next[id_itinerary];g.commit(self, id_itinerary); }

        WAIT_COMMIT -> WAIT_AGREE  { commit(sender, id_itinerary) [next[id_itinerary] /= null] /
                                        h:obj:=next[id_itinerary];h.commit(self, id_itinerary);}

        WAIT_COMMIT -> Top.RESERVED     { commit(sender, id_itinerary) [next[id_itinerary] = null and sign[id_itinerary] = null] /
                            i:obj:=prev[id_itinerary];i.agree(self, id_itinerary);}

        WAIT_COMMIT -> CHECK_LIGHT_START {  commit(sender, id_itinerary) [ next[id_itinerary] = null and sign[id_itinerary] /= null ] /
                                a:obj:= sign[id_itinerary]; a.checkGreen(id_itinerary) }

        WAIT_AGREE  -> CHECK_LIGHT_START    { agree(sender, id_itinerary) [prev[id_itinerary] /= null and sign[id_itinerary] /= null] /
                            a:obj:= sign[id_itinerary]; a.checkGreen(id_itinerary);}

        WAIT_AGREE  -> Top.RESERVED   { agree(sender, id_itinerary) [prev[id_itinerary] /= null and sign[id_itinerary] = null] /
                        l:obj:=prev[id_itinerary];l.agree(self, id_itinerary); }

        WAIT_AGREE  -> CHECK_LIGHT_START {agree(sender, id_itinerary) [prev[id_itinerary] = null and sign[id_itinerary] /= null] /
                                    a:obj:= sign[id_itinerary]; a.checkGreen(id_itinerary);
                                    }

        WAIT_AGREE  -> Top.RESERVED     { agree(sender, id_itinerary) [prev[id_itinerary] = null and sign[id_itinerary] = null] /
                            train.go;}

        CHECK_LIGHT_START  -> Top.RESERVED { green(id_itinerary)  [ prev[id_itinerary] = null and next[id_itinerary] /= null and sign[id_itinerary] /= null] /
                                train.go
                                }

        CHECK_LIGHT_START  -> Top.RESERVED { green(id_itinerary)  [ next[id_itinerary] = null and prev[id_itinerary] /= null and sign[id_itinerary] /= null] /
                                i:obj:=prev[id_itinerary];i.agree(self, id_itinerary);
                                }

        CHECK_LIGHT_START  -> Top.RESERVED { green(id_itinerary)  [ prev[id_itinerary] /= null and next[id_itinerary] /= null and sign[id_itinerary] /= null] /
                                i:obj:=prev[id_itinerary];i.agree(self, id_itinerary);
                                }

        CHECK_LIGHT_START  -> NOT_RESERVED { red(id_itinerary) [sign[id_itinerary] /= null and train /= null ] /
                                train.no_go }

        Top.RESERVED     -> NOT_RESERVED { sensorOff(sender, id_itinerary) [sign[id_itinerary] = null] /
                            train := null;}

        Top.RESERVED     -> CHECK_LIGHT_END { sensorOff(sender, id_itinerary) [sign[id_itinerary] /= null] /
                            a:obj:=sign[id_itinerary];a.checkRed(id_itinerary);
                            train := null;}

        Top.RESERVED     -> TRAIN_IN_TRANSIT { sensorOn(sender, id_itinerary) / train := sender; }

        TRAIN_IN_TRANSIT -> NOT_RESERVED { sensorOff(sender, id_itinerary) [sign[id_itinerary] = null] /
                            train := null;}

        TRAIN_IN_TRANSIT -> CHECK_LIGHT_END { sensorOff(sender,id_itinerary) [sign[id_itinerary] /= null] /
                            train := null;
                            a:obj:=sign[id_itinerary];a.checkRed(id_itinerary) }

        CHECK_LIGHT_END   -> NOT_RESERVED { red(id_itinerary) [train = null and next[id_itinerary] /= null]}

        WAIT_ACK    -> NOT_RESERVED { nack(sender, id_itinerary) /
                        if  prev[id_itinerary] /= null  then
                            { m:obj:=prev[id_itinerary];m.nack(self, id_itinerary) }
                        else
                            { train.no_go }; }

        WAIT_COMMIT -> NOT_RESERVED { disagree(sender, id_itinerary) /
                        if  next[id_itinerary] /= null  then
                            { n:obj:=next[id_itinerary];n.disagree(self, id_itinerary) }; }

        WAIT_AGREE  -> NOT_RESERVED { disagree(sender, id_itinerary) [sign[id_itinerary] = null] /
                        if  prev[id_itinerary] /= null  then
                            { o:obj:=prev[id_itinerary];o.disagree(self, id_itinerary) }
                        else
                            { train.no_go};}

        WAIT_AGREE  -> CHECK_LIGHT_START { disagree(sender, id_itinerary) [sign[id_itinerary] /= null] /
                        a:obj:=sign[id_itinerary]; a.checkRed(id_itinerary)}

        CHECK_LIGHT_START    -> NOT_RESERVED { red(id_itinerary) [prev[id_itinerary] /= null] /
                            o:obj:=prev[id_itinerary];o.disagree(self, id_itinerary);}

        Top.RESERVED     -> NOT_RESERVED { disagree(sender, id_itinerary)  [sign[id_itinerary] = null ] /
                        if next[id_itinerary] /= null then
                            { p:obj:=next[id_itinerary];p.disagree(self, id_itinerary) }
                        else
                            { p:obj:=prev[id_itinerary];p.nack(self, id_itinerary) };}

        Top.RESERVED     -> NOT_RESERVED { nack(sender, id_itinerary) [prev[id_itinerary] /= null] /
                            p:obj:=prev[id_itinerary];p.nack(self,id_itinerary);
                            }
        Top.RESERVED     -> CHECK_LIGHT_END { disagree(sender, id_itinerary) [sign[id_itinerary] /= null and next[id_itinerary] = null ]  /
                        o:obj:=sign[id_itinerary];o.checkRed(id_itinerary);}

        Top.RESERVED     -> CHECK_LIGHT_END { disagree(sender, id_itinerary)  [sign[id_itinerary] /= null and next[id_itinerary] /= null ]  /
                            p:obj:=next[id_itinerary];p.disagree(self, id_itinerary) ;
                            o:obj:=sign[id_itinerary];o.checkRed(id_itinerary);}

        CHECK_LIGHT_END -> NOT_RESERVED {  red(id_itinerary) [next[id_itinerary] = null] /
                                        p:obj:=prev[id_itinerary];p.nack(self,id_itinerary);}

        NOT_RESERVED -> NOT_RESERVED { nack(sender, id_itinerary) /
                                        o:obj:=prev[id_itinerary];o.nack(self, id_itinerary);}
        NOT_RESERVED -> NOT_RESERVED { req(sender, id_itinerary) [train/=null and sender/=train] /
                        sender.nack(self, id_itinerary); }

        WAIT_ACK        -> WAIT_ACK        { req(sender, id_itinerary) / sender.nack(self, id_itinerary); }
        WAIT_COMMIT     -> WAIT_COMMIT     { req(sender, id_itinerary) / sender.nack(self, id_itinerary); }
        WAIT_AGREE      -> WAIT_AGREE      { req(sender, id_itinerary) / sender.nack(self, id_itinerary); }
        Top.RESERVED     -> Top.RESERVED     { req(sender, id_itinerary) / sender.nack(self, id_itinerary); }
        TRAIN_IN_TRANSIT -> TRAIN_IN_TRANSIT { req(sender, id_itinerary) / sender.nack(self, id_itinerary); }

    end Linear

    class Signal is
        Signals:
            checkGreen(id_itinerary);
            checkRed(id_itinerary);

        Vars:
            linear :obj;
            red : bool:= true;

        State Top = RED , GREEN
        State GREEN Defers checkRed(id_itinerary)
        State RED Defers checkRed(id_itinerary)

        Transitions:
        RED -> GREEN { checkGreen(id_itinerary) [red = true] /
                        linear.green(id_itinerary);
                            red = false; }
        RED -> RED { checkRed(id_itinerary) [red = true] /
                            linear.red(id_itinerary);
                            red = true}

        GREEN -> RED { checkRed(id_itinerary)  [red = false] /
                            linear.red(id_itinerary);
                            red = true;}

        GREEN -> GREEN { checkGreen(id_itinerary) [red = false] /
                            linear.green(id_itinerary);
                            }
    end Signal

    Class Point is

    Signals:
        req(sender: obj, id_itinerary: int);
        ack(sender: obj, id_itinerary: int);
        nack(sender: obj, id_itinerary: int);
        commit(sender: obj, id_itinerary: int);
        agree(sender: obj, id_itinerary: int);
        disagree(sender: obj, id_itinerary: int);

    Operations:
        sensorOn(sender: obj, id_itinerary);
        sensorOff(sender: obj, id_itinerary);

    Vars:
        next: obj[];
        prev: obj[];
        conf: bool[];
        reverse: bool := False;
        train: obj := null;
        itinerary: int;

    State Top = NOT_RESERVED, WAIT_ACK, WAIT_COMMIT, WAIT_AGREE, POSITIONING, RESERVED, CHECK_NOT_RESERVED, TRAIN_IN_TRANSIT
    State WAIT_ACK Defers req(sender: obj, id_itinerary: int)
    State WAIT_ACK Defers nack(sender,id_itinerary)

    Transitions:

        NOT_RESERVED  -> WAIT_ACK     { req(sender, id_itinerary) /
                    q:obj:=next[id_itinerary];q.req(self, id_itinerary); }

        WAIT_ACK     -> WAIT_COMMIT  { ack(sender, id_itinerary) /
                    r:obj:=prev[id_itinerary];r.ack(self, id_itinerary); }

        WAIT_COMMIT  -> WAIT_AGREE   { commit(sender, id_itinerary) /
                    s:obj:=next[id_itinerary];s.commit(self, id_itinerary); }

        WAIT_AGREE   -> Top.RESERVED      { agree(sender, id_itinerary) [reverse = conf[id_itinerary]]/
                                        t:obj:=prev[id_itinerary];t.agree(self, id_itinerary); }

        WAIT_AGREE   -> POSITIONING { agree(sender, id_itinerary) [reverse /= conf[id_itinerary]] /
                                            itinerary := id_itinerary;}

        POSITIONING -> Top.RESERVED      { - /
                                            reverse := not reverse;
                        u:obj:=prev[itinerary];u.agree(self, itinerary); }

        POSITIONING -> CHECK_NOT_RESERVED { - /
                        z:obj:=next[itinerary];z.disagree(self, itinerary);}

        CHECK_NOT_RESERVED -> NOT_RESERVED { nack(sender,id_itinerary) /
                            v:obj:=prev[id_itinerary];v.disagree(self, id_itinerary);}

        NOT_RESERVED    -> NOT_RESERVED { nack(sender,id_itinerary)/
                            v:obj:=prev[id_itinerary];v.nack(self,id_itinerary);}

        Top.RESERVED     -> TRAIN_IN_TRANSIT { sensorOn(sender, id_itinerary) / train := sender; }

        TRAIN_IN_TRANSIT -> NOT_RESERVED { sensorOff(sender, id_itinerary) /
                                            train := null;}

        WAIT_ACK     -> NOT_RESERVED { nack(sender, id_itinerary) /
                            a:obj:=prev[id_itinerary];a.nack(self, id_itinerary); }

        WAIT_COMMIT  -> NOT_RESERVED { disagree(sender, id_itinerary) /
                            b:obj:=next[id_itinerary];b.disagree(self, id_itinerary); }

        WAIT_AGREE   -> NOT_RESERVED { disagree(sender, id_itinerary) /
                            c:obj:=prev[id_itinerary];c.disagree(self, id_itinerary); }

        POSITIONING -> NOT_RESERVED { disagree(sender, id_itinerary) /
                            d:obj:=next[id_itinerary];d.disagree(self, id_itinerary); }

        Top.RESERVED      -> NOT_RESERVED { disagree(sender, id_itinerary) /
                            e:obj:=next[id_itinerary];e.disagree(self, id_itinerary); }

        WAIT_COMMIT     -> WAIT_COMMIT     { req(sender, id_itinerary) / sender.nack(self, id_itinerary); }
        WAIT_AGREE      -> WAIT_AGREE      { req(sender, id_itinerary) / sender.nack(self, id_itinerary); }
        POSITIONING    -> POSITIONING    { req(sender, id_itinerary) / sender.nack(self, id_itinerary); }
        Top.RESERVED     -> Top.RESERVED     { req(sender, id_itinerary) / sender.nack(self, id_itinerary); }
        TRAIN_IN_TRANSIT -> TRAIN_IN_TRANSIT { req(sender, id_itinerary) / sender.nack(self, id_itinerary); }

    end Point
    )";

};
