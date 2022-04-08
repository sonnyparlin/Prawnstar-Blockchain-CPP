import React from 'react';
import { Switch ,Route } from 'react-router-dom';
import {AppProvider} from './AppContext';
import News from './News';
import Transactions from './Transactions';
import Wallet from './Wallet';
import BuySellTransfer from './BuySellTransfer';
import Console from './Console';
import Stake from './Stake';

function Prawnstar (props) {
    const {path} = props.match;
    return (
        <AppProvider>
            <Switch>
                <Route exact path={path} element={<News />} />
                <Route exact path={path} element={<Transactions/>} />
                <Route exact path={path} element={<BuySellTransfer/>} />
                <Route exact path={path} element={<Wallet/>} />
                <Route exact path={path} element={<Stake/>} />
                <Route exact path={path} element={<Console />} />
            </Switch>
        </AppProvider>
    );
}

export default Prawnstar;