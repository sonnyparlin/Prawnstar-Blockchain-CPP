import React from 'react';
import $ from 'jquery';

class News extends React.Component {

    constructor(props) { 
        super(props);
        this.state = { news: [{},{}] };
    }

    componentDidMount() {
        this.getNews();
    }

    getNews() {

        $.get("https://cointelegraph.com/feed",function(data) {    

            var $xml = $(data);   
            var items = [];

            $xml.find("item").each(function() {
                var $this = $(this);
                items.push({
                    title: $this.find("title").text(),
                    link: $this.find("link").text(),
                    description: $this.find("description").text(),
                    pubDate: $this.find("pubDate").text(),
                });
            });

            this.setState({ news: items });

        }.bind(this),'xml');
    }

    render() { 
        return (
        <div className="App-feeds">
            <div className="panel-list">
                <small>
                <a target="_new" href={this.state.news[0] ? this.state.news[0].link : null}>{this.state.news[0] ? this.state.news[0].title : null}</a><br/><small>{this.state.news[0] ? this.state.news[0].pubDate : null}</small><br/><br/>

                <a target="_new" href={this.state.news[1] ? this.state.news[1].link : null}>{this.state.news[1] ? this.state.news[1].title : null}</a><br/><small>{this.state.news[1] ? this.state.news[1].pubDate : null}</small><br/><br/>

                <a target="_new" href={this.state.news[2] ? this.state.news[2].link : null}>{this.state.news[2] ? this.state.news[2].title : null}</a><br/><small>{this.state.news[2] ? this.state.news[2].pubDate : null}</small><br/><br/>

                <a target="_new" href={this.state.news[3] ? this.state.news[3].link : null}>{this.state.news[3] ? this.state.news[3].title : null}</a><br/><small>{this.state.news[3] ? this.state.news[3].pubDate : null}</small><br/><br/>

                <a target="_new" href={this.state.news[4] ? this.state.news[4].link : null}>{this.state.news[4] ? this.state.news[4].title : null}</a><br/><small>{this.state.news[4] ? this.state.news[4].pubDate : null}</small><br/><br/>

                <a target="_new" href={this.state.news[5] ? this.state.news[5].link : null}>{this.state.news[5] ? this.state.news[5].title : null}</a><br/><small>{this.state.news[5] ? this.state.news[5].pubDate : null}</small><br/><br/>

                <a target="_new" href={this.state.news[6] ? this.state.news[6].link : null}>{this.state.news[6] ? this.state.news[6].title : null}</a><br/><small>{this.state.news[6] ? this.state.news[6].pubDate : null}</small><br/><br/>

                <a target="_new" href={this.state.news[7] ? this.state.news[7].link : null}>{this.state.news[7] ? this.state.news[7].title : null}</a><br/><small>{this.state.news[7] ? this.state.news[7].pubDate : null}</small><br/><br/>

                <a target="_new" href={this.state.news[8] ? this.state.news[8].link : null}>{this.state.news[8] ? this.state.news[8].title : null}</a><br/><small>{this.state.news[8] ? this.state.news[8].pubDate : null}</small><br/><br/>

                <a target="_new" href={this.state.news[9] ? this.state.news[9].link : null}>{this.state.news[9] ? this.state.news[9].title : null}</a><br/><small>{this.state.news[9] ? this.state.news[9].pubDate : null}</small><br/><br/>
                </small>
            </div>
        </div>
        );
    }
}

export default News;