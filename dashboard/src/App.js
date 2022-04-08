import React from 'react';
import {BrowserRouter as Router, Switch, Route} from 'react-router-dom';
import Prawnstar from './Prawnstar';
import Home from './Home';
import './App.css';

export default function App() {
    return (
        <Router>
            <div>
                <Switch>
                    <Route exact path="/" element={<Home />} />
                    <Route path="/Prawnstar" element={<Prawnstar />} />
                </Switch>
            </div>
        </Router>
    );
}

// class App extends React.Component {

    

//     render() { 
//         return (
//             <div className="container-lg">
//                 <div className="col-4">
//                     <div className="p-2 border bg-white info">
//                        <NodeInfo />
                
//                     </div>

//                     <div className="p-2 border bg-white news">
//                         <b>Crypto News:</b><br/>
//                         <News/>
//                     </div>
//                 </div>  

//                 <div className="col-4">
//                     <div className="p-2 border bg-white txBox">
//                         <b>Transaction History</b>
//                         <div className="txhistory">
//                             <Transactions/>
//                         </div>
//                     </div>

//                     <div className="p-2 border bg-white buysell">
//                         <b>Buy / Sell</b>
//                         <BuySellTransfer/>
//                     </div>
//                 </div>        

//                 <div className="col-4">
//                     <div className="p-2 border bg-white wallet">
//                         <b>Wallet</b>
//                         <Wallet/>
//                     </div>
//                     <div className="p-2 border bg-white stake">
//                         <b>Stake</b>
//                         <Stake/>
//                     </div>
//                     <div className="p-2 border bg-white">
//                         <b>Node Console</b>
//                         <div className="console">
//                             <Console/>
//                         </div>
//                     </div>
//                 </div>

//             </div>
//         );
//     }
// }

// export default App;
